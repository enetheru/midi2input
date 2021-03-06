//system includes
#include <chrono>
#include <thread>
#include <csignal>
#include <filesystem>
namespace fs = std::filesystem;

//library includes
extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}
#include "argh.h"
#include <spdlog/spdlog.h>

//local includes
#include "util.h"
#include "midi.h"
#include "inotify.h"
#include "uinput.h"
#include "lua.h"

#ifdef WITH_JACK
    #include "jack.h"
#endif//WITH_JACK

#ifdef WITH_ALSA
    #include "alsa.h"
#endif//WITH_ALSA

#ifdef WITH_QT
    #include <QApplication>
    #include <QSystemTrayIcon>
    #include "trayicon.h"
#endif//WITH_QT

#ifdef WITH_XORG
    #include "x11.h"
#endif//WITH_XORG

namespace m2i {
    const char *helptext =
"USAGE: ./m2i [options]\n"
"OPTIONS:\n"
"   -h  --help                  Print usage and exit\n"
"   -v  --verbose               Output more information\n"
"   -q  --quiet                 Output less information\n"
"   -c  --config <config.lua>   Specify config file\n"
"   -s  --script <script.lua>   Specify script file\n"
"   -a  --alsa                  Use ALSA midi backend\n"
"   -j  --jack                  Use Jack midi backend\n";
//decided that unless it seems paramount, then further configuration should go
//into the config file

    /* ====================== Global Options ============================ */
    // Yuck i know haha.. oh well.
    int loglevel = 100;
    bool use_alsa = true;
    bool use_jack = false;
    bool reconnect = true;
    bool loop_enabled = true;
    std::chrono::milliseconds main_freq( 10 );
    std::chrono::milliseconds loop_freq( 100 );
    std::chrono::milliseconds watch_freq( 1000 );
    fs::path config = "config.lua";
    fs::path script = "script.lua";

    //program state
    lua_State *L = nullptr;
    Notifier notifier;
    bool quit = false;

    #ifdef WITH_ALSA
    AlsaSeq seq;
    #endif//WITH_ALSA

    #ifdef WITH_JACK
    JackSeq jack;
    #endif//WITH_JACK

    //uinput driver
    m2i::Uinput uinput;

}//end namespace m2i

//signal interrupt handler for ctrl+c
static void
intHandler( int dummy ){
    (void)dummy;
    m2i::quit = true;
}

static void
loadConfig( lua_State *L, const fs::path &path ){
     // Load configuraton lua script
    spdlog::info( FMT_STRING( "LUA: Loading configuration: {}" ), path.c_str() );
    if( luaL_loadfile( L, path.c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
        spdlog::error( FMT_STRING( "LUA: failure loading configuration file: {}" ), lua_tostring( L, -1 ) );
        lua_pop( L, 1 );
        return;
    }

    //pull configuration from config
    lua_getglobal( L, "config" );
    if( !lua_istable(L, -1 ) ){
        spdlog::error( FMT_STRING( "LUA: No 'config' table found in {}" ), path.c_str() );
        lua_pop( L, 1 );
        return;
    }

    // Collect unused properties into vector, to warn about them later.
    // This is done in batch to ensure that we honor loglevel which possibly mutes warnings.
    std::vector<std::string> unrecognized_vars{};

    lua_pushnil( L );
    while( lua_next( L, -2 ) != 0 ){
        std::string var( lua_tostring( L, -2 ) );
        if(      var == "script"       )m2i::script = lua_tostring( L, -1 ) ;
        else if( var == "loglevel"     ){
            //Check spdlog/common.h:125 for numbers and labels
            spdlog::set_level( static_cast<spdlog::level::level_enum>(lua_tointeger( L, -1 )) );
        }
        else if( var == "use_alsa"     )m2i::use_alsa = lua_toboolean( L, -1 );
        else if( var == "use_jack"     )m2i::use_jack = lua_toboolean( L, -1 );
        else if( var == "reconnect"    )m2i::reconnect = lua_toboolean( L, -1 );
        else if( var == "loop_enabled" )m2i::loop_enabled = lua_toboolean( L, -1 );
        else if( var == "main_freq"    )m2i::main_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
        else if( var == "loop_freq"    )m2i::loop_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
        else if( var == "watch_freq"   )m2i::watch_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
        else unrecognized_vars.push_back( std::move( var ) );
        lua_pop( L, 1 );
    }
    lua_pop( L, 1 ); //lua_getglobal "config"

    for( const std::string &var : unrecognized_vars ){
        spdlog::warn( FMT_STRING( "CONFIG: Unrecognized configuration key: {}" ), var );
    }
}

static void
inotify_script_change()
{
    spdlog::info( FMT_STRING( "restarting lua" ) );
    //blow away the lua state
    if( m2i::L )lua_close( m2i::L );
    m2i::L = nullptr;

    //start from scratch
    m2i::L = luaL_newstate();
    luaL_openlibs( m2i::L );

    //register out functions with lua
    m2i::register_lua_funcs(m2i::L);

    spdlog::info( FMT_STRING( "LUA: Loading script: {}" ), m2i::script.c_str() );
    if( luaL_loadfile( m2i::L, m2i::script.c_str() ) || lua_pcall( m2i::L, 0, 0, 0 ) ){
        spdlog::error( FMT_STRING( "LUA: failure loading script file: {}" ), lua_tostring( m2i::L, -1 ) );
        lua_pop( m2i::L, 1);
        return;
    }

    lua_getglobal( m2i::L, "script_init" );
    if( lua_pcall( m2i::L, 0, 0, 0 ) != LUA_OK ){
        spdlog::warn( FMT_STRING("LUA: Missing function: {}"), lua_tostring( m2i::L, -1) );
        lua_pop( m2i::L, 1);
    }
}

int
main( int argc, char **argv )
{
    //handle ctrl+c to exit the main loop.
    signal(SIGINT, intHandler);

    /* ============================ Argh Parser ========================= */
    //add key/value pairs
    argh::parser cmdl( { "-c", "--config", "-s", "--script" } );
    cmdl.parse( argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION );

    if( cmdl[{"-h", "--help"   }] ){
        fmt::print( m2i::helptext );
        exit( 0 );
    }
    if( cmdl[{"-v", "--verbose"}] )m2i::loglevel = 100;
    if( cmdl[{"-q", "--quiet"  }] )m2i::loglevel = 1;
    if( cmdl({"-c", "--config" }) )
        m2i::config = m2i::getPath( cmdl({"-c", "--config"}).str() );
    else
        m2i::config = m2i::getPath( "config.lua" );

    /* =================== Lua Initialisation ================= */
    m2i::L = luaL_newstate();
    luaL_openlibs( m2i::L );
    m2i::register_lua_funcs(m2i::L);

    /* === Load Configuration === */
    // We only do this once on program load, reloading the script does not reload configuration items.
    loadConfig( m2i::L, m2i::config );

    //command line overrides
    if( cmdl[{"-a", "--alsa"   }] )m2i::use_alsa = true;
    if( cmdl[{"--no-alsa"      }] )m2i::use_alsa = false;
    if( cmdl[{"-j", "--jack"   }] )m2i::use_jack = true;
    if( cmdl[{"--no-jack"      }] )m2i::use_jack = false;
    if( cmdl({"-s", "--script" }) )m2i::script = cmdl({"-s", "--script"}).str();
    m2i::script = m2i::getPath( m2i::script );

    //check that we at least use one midi backend, otherwise there is kinda no point
    if( !m2i::use_alsa && !m2i::use_jack ){
        spdlog::error( FMT_STRING( "neither jack nor alsa has been specified" ) );
        exit(-1);
    }

    /* ============================== ALSA ============================== */
    if( m2i::use_alsa ){
    #ifdef WITH_ALSA
        m2i::seq.open();
    #else
        spdlog::error( FMT_STRING( "Not compiled with ALSA midi backend" ) );
        exit(-1);
    #endif
    }

    /* ============================== Jack ============================== */
    if( m2i::use_jack ){
    #ifdef WITH_JACK
        m2i::jack.init();
    #else
        spdlog::error( FMT_STRING( "Not compiled with Jack midi backend" ) );
        exit(-1);
    #endif
    }

    /* ============================= X11 ================================ */
    #ifdef WITH_XORG
    Display *xdp;
    if(! (xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
        spdlog::error( FMT_STRING( "Unable to open X display" ) );
        exit( -1 );
    }
    XCloseDisplay( xdp );
    XSetErrorHandler( m2i::XErrorCatcher );
    #endif

    /* ======================== QT System Tray ========================== */
    #ifdef WITH_QT
    QApplication app(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        spdlog::error( FMT_STRING( "system tray unavailable" ) );
        exit( -1);
    }

    m2iTrayIcon myIcon;
    QApplication::setQuitOnLastWindowClosed(false);

    #endif//WITH_QT

    /* ========================== Load Script =========================== */
    spdlog::info( FMT_STRING( "LUA: Loading script: {}" ), m2i::script.c_str() );
    if( luaL_loadfile( m2i::L, m2i::script.c_str() ) || lua_pcall( m2i::L, 0, 0, 0 ) ){
        spdlog::critical( FMT_STRING( "LUA: failure loading script file: {}" ), lua_tostring( m2i::L, -1 ) );
        lua_pop( m2i::L, 1 );
        return -1;
    } else {
        m2i::notifier.watchPath({0, m2i::script, inotify_script_change});
    }

    lua_getglobal( m2i::L, "script_init" );
    if( lua_pcall( m2i::L, 0, 0, 0 ) != LUA_OK ){
        spdlog::warn( FMT_STRING("LUA: Missing function: {}"), lua_tostring( m2i::L, -1) );
        lua_pop( m2i::L, 1);
    }

    /* =========================== Main Loop ============================ */
    spdlog::info( FMT_STRING( "Entering sleep, waiting for events" ) );
    std::chrono::system_clock::time_point loop_last = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point watch_last = std::chrono::system_clock::now();
    while(! m2i::quit )
    {
        auto main_start = std::chrono::system_clock::now();

        //this is the way it must be done if i want it to be completely optional
        #ifdef WITH_QT
        QCoreApplication::processEvents();
        #endif

        #ifdef WITH_ALSA
        if( m2i::seq ){
            while( m2i::seq.event_pending() > 0 ){
                m2i::midi_to_lua(m2i::L, m2i::seq.event_receive());
            }
        }
        #endif//WITH_ALSA

        #ifdef WITH_JACK
        if( m2i::jack.valid ){
            while( m2i::jack.event_pending() > 0 ){
                m2i::midi_to_lua(m2i::L, m2i::jack.event_receive());
            }
        }
        #endif//WITH_JACK

        // run regular checks at watch_freq
        auto watch_wait = std::chrono::system_clock::now() - watch_last;
        if( watch_wait > m2i::watch_freq ){
            watch_last = std::chrono::system_clock::now();

            //notifier checks for modified files and runs the functions that
            //have been associated with them.
            m2i::notifier.check();

            #ifdef WITH_JACK
            if( m2i::use_jack && m2i::reconnect && !m2i::jack.valid ){
                // FIXME I'm not really happy with the re-initialisation of jack
                spdlog::error( FMT_STRING( "Jack not valid attempting to re-initialise" ) );
                //attempt to re-instantiate jack connection
                m2i::jack.fina();
                m2i::jack.init();
            }
            #endif//WITH_JACK

            {
                int luastacksize = lua_gettop(m2i::L);
                if( luastacksize != 0 ) spdlog::info( FMT_STRING( "Lua Stack Size: {}" ), luastacksize);
            }
        }

        // run the loop lua function at loop_freq
        auto loop_wait = std::chrono::system_clock::now() - loop_last;
        if( m2i::loop_enabled && loop_wait > m2i::loop_freq ){
            loop_last = std::chrono::system_clock::now();
            lua_getglobal( m2i::L, "loop" );
            if( lua_pcall( m2i::L, 0, 0, 0 ) != LUA_OK ){
                spdlog::warn( FMT_STRING("LUA: Missing function: {}"), lua_tostring( m2i::L, -1) );
                spdlog::error( FMT_STRING("LUA: Disabling loop function") );
                m2i::loop_enabled = false;
                lua_pop( m2i::L, 1 );
            }
        }

        //limit mainloop to m2i::main_freq
        auto main_end = std::chrono::system_clock::now();
        std::chrono::duration< double > main_time = main_end - main_start;
        if( main_time > m2i::main_freq ){
            spdlog::warn( FMT_STRING( "processing time of {:.6f}ms is longer than timer resolution" ),
                     main_time.count() * 1000 );
        } else {
            std::this_thread::sleep_for( m2i::main_freq - main_time );
        }
    }

    lua_close( m2i::L );
    exit( 0 );
}
