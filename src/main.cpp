//system includes
#include <chrono>
#include <thread>
#include <signal.h>
#include <functional>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

//library includes
extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}
#include "argh.h"

//local includes
#include "main.h"
#include "util.h"
#include "cache_data.h"
#include "midi.h"
#include "inotify.h"

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
    #include <QMessageBox>
    #include "trayicon.h"
#endif//WITH_QT

#ifdef WITH_XORG
    #include "x11.h"
#endif//WITH_XORG

/* FIXME cleanup namespacing across the project, everything i create should be
 * in the m2i namespace maybe turn everything into a class, and pass the object
 * through the function calls
 * eg.
class mainObj {
    //blah
};
*/

namespace m2i {
    const char *helptext =
"USAGE: ./m2i [options]\n"
"OPTIONS:\n"
"   -h  --help                  Print usage and exit\n"
"   -v  --verbose               Output more information\n"
"   -c  --config <config.lua>   Specify config file\n"
"   -s  --script <script.lua>   Specify script file\n"
"   -a  --alsa                  Use ALSA midi backend\n"
"   -j  --jack                  Use Jack midi backend\n";
//decided that unless it seems paramount, then further configuration should go
//into the config file

    /* ====================== Global Options ============================ */
    // Yuck i know haha.. oh well.
    int loglevel = 5;
    bool use_alsa = true;
    bool use_jack = false;
    bool use_xorg = true;
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
    snd::Seq seq;
    #endif//WITH_ALSA

    #ifdef WITH_JACK
    JackSeq jack;
    #endif//WITH_XORG


//signal interrupt handler for ctrl+c
void intHandler( int dummy ){
    (void)dummy;
    m2i::quit = true;
}

//TODO jack error handler here

void
loadConfig( lua_State *L, const fs::path &path ){
     // Load configuraton lua script
    if( m2i::lua_loadscript( L, path ) < 0 ){
        LOG( ERROR ) << "unable to load config: " << path << "\n";
        return;
    }

    //pull configuration from config
    lua_getglobal( L, "config" );
    if( !lua_istable(L, -1 ) ){
        LOG( ERROR ) << "No 'config' table found in lua file\n";
        lua_pop( L, 1 );
        return;
    }

    cacheSet( "config", path.string() );

    lua_pushnil( L );
    while( lua_next( L, -2 ) != 0 ){
        std::string var( lua_tostring( L, -2 ) );
             if( var == "script"       )m2i::script = lua_tostring( L, -1 ) ;
        else if( var == "loglevel"     )m2i::loglevel = lua_tointeger( L, -1 );
        else if( var == "use_alsa"     )m2i::use_alsa = lua_toboolean( L, -1 );
        else if( var == "use_jack"     )m2i::use_jack = lua_toboolean( L, -1 );
        else if( var == "reconnect"    )m2i::reconnect = lua_toboolean( L, -1 );
        else if( var == "loop_enabled" )m2i::loop_enabled = lua_toboolean( L, -1 );
        else if( var == "main_freq"    )m2i::main_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
        else if( var == "loop_freq"    )m2i::loop_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
        else if( var == "watch_freq"   )m2i::watch_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
        lua_pop( L, 1 );
    }
    lua_pop( L, 1 ); //lua_getglobal "config"
    return;
}

void restartLua()
{
    LOG( INFO ) << "restarting lua\n";
    //blow away the lua state
    if( !L )lua_close( m2i::L );
    L = nullptr;
    //FIXME unset inotify watches on old files;
    m2i::loop_enabled = true;

    //start from scratch
    m2i::L = m2i::lua_init_new();
    if( m2i::lua_loadscript( m2i::L, m2i::script ) < 0 ){
        LOG( ERROR ) << "Unable to find script file:" << m2i::script << "\n";
        return;
    }

    lua_getglobal( L, "script_init" );
    if( lua_pcall( L, 0, 0, 0 ) != 0 )lua_pop( L, 1);
}

}//end namespace m2i

using namespace m2i;

int
main( int argc, char **argv )
{
    //handle ctrl+c to exit the main loop.
    signal(SIGINT, intHandler);

    /* ================================================================== */
    //add key/value pairs
    argh::parser cmdl( { "-c", "--config", "-s", "--script" } );
    cmdl.parse( argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION );

    if( cmdl[{"-h", "--help"   }] ){
        LOG(INFO) << helptext << "\n";
        exit( 0 );
    }
    if( cmdl[{"-v", "--verbose"}] )m2i::loglevel = 5;
    if( cmdl[{"-q", "--quiet"  }] )m2i::loglevel = 1;
    if( cmdl({"-c", "--config" }) )
        m2i::config = m2i::getPath( cmdl({"-c", "--config"}).str() );
    else
        m2i::config = m2i::getPath( "config.lua" );

    /* ================================================================== */
    m2i::L = m2i::lua_init_new();
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
        LOG( ERROR ) << "neither jack nor alsa has been specified\n";
        exit(-1);
    }

    /* ================================================================== */
    //load script
    if( m2i::lua_loadscript( m2i::L, m2i::script ) < 0 ){
        LOG( ERROR ) << "Unable to find script file:" << m2i::script << "\n";
    } else {
        cacheSet( "script", m2i::script );
        m2i::notifier.watchPath( { m2i::script, restartLua } );

        lua_getglobal( L, "script_init" );
        if( lua_pcall( L, 0, 0, 0 ) != 0 )lua_pop( L, 1);
    }

    /* ============================== ALSA ============================== */
    if( m2i::use_alsa ){
    #ifdef WITH_ALSA
        m2i::seq.open();
    #else
        LOG( ERROR ) << "Not compiled with ALSA midi backend\n";
        exit(-1);
    #endif
    }

    /* ============================== Jack ============================== */
    if( m2i::use_jack ){
    #ifdef WITH_JACK
        m2i::jack.init();
    #else
        LOG( ERROR ) << "Not compiled with Jack midi backend\n";
        exit(-1);
    #endif
    }

    /* ============================= X11 ================================ */
    #ifdef WITH_XORG
    LOG( INFO ) << "Getting X11 Display\n";
    Display *xdp;
    if(! (xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
        LOG( FATAL ) << "Unable to open X display\n";
        exit( -1 );
    }
    XCloseDisplay( xdp );
    XSetErrorHandler( m2i::XErrorCatcher );
    #endif

    /* ======================== QT System Tray ========================== */
    #ifdef WITH_QT
    QApplication app(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        LOG( ERROR ) << "system tray unavailable\n";
        exit( -1);
    }

    m2iTrayIcon myIcon;
    QApplication::setQuitOnLastWindowClosed(false);

    #endif//WITH_QT
    /* =========================== Main Loop ============================ */
    LOG( INFO ) << "Main: Entering sleep, waiting for events\n";
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
                m2i::lua_midirecv( m2i::L, m2i::seq.event_receive() );
            }
        }
        #endif//WITH_ALSA

        #ifdef WITH_JACK
        if( m2i::jack.valid ){
            while( m2i::jack.event_pending() > 0 ){
                m2i::lua_midirecv( m2i::L, m2i::jack.event_receive() );
            }
        }
        #endif//WITH_JACK

        // run regular checks at watch_freq
        auto watch_wait = std::chrono::system_clock::now() - watch_last;
        if( watch_wait > m2i::watch_freq ){
            watch_last = std::chrono::system_clock::now();

            notifier.check();

            #ifdef WITH_JACK
            if( m2i::use_jack && m2i::reconnect && !m2i::jack.valid ){
                // FIXME I'm not really happy with the re-initialisation of jack
                LOG( ERROR ) << "Jack not valid attempting to re\n";
                //attempt to re-instantiate jack connection
                m2i::jack.fina();
                m2i::jack.init();
                //TODO reconnect to previously connected ports.
            }
            #endif//WITH_JACK
            //TODO inotify to monitor and reload configuration
            //TODO update cache for connected ports

            #ifdef WITH_ALSA
            //reconnect to alsa ports
            #endif//WITH_ALSA

            #ifdef WITH_XORG
            #endif//WITH_XORG
            int luastacksize;
            if( (luastacksize = lua_gettop( L )) ) LOG( INFO ) << "Lua Stack Size: " <<  luastacksize << "\n";

        }

        // run the loop lua function at loop_freq
        auto loop_wait = std::chrono::system_clock::now() - loop_last;
        if( m2i::loop_enabled && loop_wait > m2i::loop_freq ){
            loop_last = std::chrono::system_clock::now();
            lua_getglobal( L, "loop" );
            if( lua_pcall( L, 0, 0, 0 ) != 0 ){
                LOG( ERROR ) << "loop function call failed, disabling\n"; 
                m2i::loop_enabled = false;
            }
        }

        //limit mainloop to m2i::main_freq
        auto main_end = std::chrono::system_clock::now();
        std::chrono::duration< double > main_time = main_end - main_start;
        if( main_time > m2i::main_freq )
            LOG( WARN ) << "processing time( " << std::setprecision(2) << main_time.count() * 1000 << "ms ) is longer than timer resolution\n";
        else
            std::this_thread::sleep_for( m2i::main_freq - main_time );
    }

    lua_close( L );
    exit( 0 );
}


