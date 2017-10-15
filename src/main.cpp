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

#include "lua.h"

#ifdef WITH_JACK
    #include "jack.h"
#endif//WITH_JACK

#ifdef WITH_ALSA
    #include "alsa.h"
#endif//WITH_ALSA

#ifdef WITH_XORG
    #include "x11.h"
#endif//WITH_XORG

namespace m2i {
    //configuration
    int loglevel = 2;
    bool verbose = false;
    bool quiet = false;
    bool use_alsa = true;
    bool use_jack = false;
    bool use_xorg = true;
    bool reconnect = true;
    bool loop_enabled = true;
    std::chrono::milliseconds main_freq( 25 );
    std::chrono::milliseconds loop_freq( 100 );
    std::chrono::milliseconds watch_freq( 3000 );
    fs::path config( "config.lua" );
    fs::path script( "script.lua" );

    //program state
    lua_State *L = nullptr;
    bool quit = false;
    #ifdef WITH_ALSA
    AlsaSeq alsa;
    #endif//WITH_ALSA

    #ifdef WITH_JACK
    JackSeq jack;
    #endif//WITH_XORG
}

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

//signal interrupt handler for ctrl+c
void intHandler( int dummy ){
    (void)dummy;
    m2i::quit = true;
}

//TODO jack error handler here

using namespace m2i;

int
main( int argc, const char **argv )
{
    //handle ctrl+c to exit the main loop.
    signal(SIGINT, intHandler);

    argh::parser cmdl( { "-c", "--config", "-s", "--script" } );
    cmdl.parse( argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION );

    if( cmdl[{"-h", "--help"}] ){
        LOG(INFO) << helptext << "\n";
        exit( 0 );
    }

    //load lua environment first
    /* ============================== Lua =============================== */
    lua_State *L = luaL_newstate();
    m2i::L = L;
    luaL_openlibs( L );
    m2i::lua_init_new( L );

    // Load configuraton lua script
    cmdl({"-c", "--config"}) >> m2i::config;
    if( m2i::lua_loadscript( L, m2i::config ) < 0 ){
        LOG( ERROR ) << "unable to load config: " << m2i::config << "\n";
        LOG( WARN ) << "using default configuration\n";
    } else {
        cacheSet( "config", m2i::config );

        //pull configuration from config.lua
        lua_getglobal( L, "config" );
        if( lua_istable(L, -1 ) ){
            lua_pushnil(L);

            while( lua_next( L, -2 ) != 0 ){
                std::string var( lua_tostring( L, -2 ) );
                     if( var == "script"   ) m2i::script   = lua_tostring( L, -1 );
                else if( var == "verbose"  ) m2i::verbose  = lua_toboolean( L, -1 );
                else if( var == "quiet"    ) m2i::quiet    = lua_toboolean( L, -1 );
                else if( var == "use_alsa" ) m2i::use_alsa = lua_toboolean( L, -1 );
                else if( var == "use_jack" ) m2i::use_jack = lua_toboolean( L, -1 );
                else if( var == "reconnect" ) m2i::reconnect = lua_toboolean( L, -1 );
                else if( var == "loop_enabled" ) m2i::loop_enabled = lua_toboolean( L, -1 );
                else if( var == "main_freq" ) m2i::main_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
                else if( var == "loop_freq" ) m2i::loop_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );
                else if( var == "watch_freq" ) m2i::watch_freq = std::chrono::milliseconds( lua_tointeger( L, -1 ) );

                lua_pop( L, 1 );
            }
        }
        else {
            LOG( ERROR ) << "No 'config' table found in lua file\n";
        }
        lua_pop( L, 1 ); //lua_getglobal "config"
    }

    /* ============================= cmdl =============================== */
    //cmdl overrides
    if( cmdl[{ "-v", "--verbose" }] )
        m2i::loglevel = 5;

    if( cmdl[{ "-q", "--quiet" }] )
        m2i::loglevel = 1;

    if( cmdl[{ "-a", "--alsa" }] )
        m2i::use_alsa = true;

    if( cmdl[{ "-j", "--jack" }] )
        m2i::use_jack = true;

    if( !cmdl({"-s", "--script"}).str().empty() )
        cmdl({"-s", "--script"}) >> m2i::script;
    if( m2i::lua_loadscript( L, m2i::script ) < 0 ){
        LOG( ERROR ) << "Unable to find script file:" << m2i::script << "\n";
    } else {
        cacheSet( "script", m2i::script );
    }

    //one final check before continuing
    if( !m2i::use_alsa && !m2i::use_jack ){
        LOG( ERROR ) << "neither jack nor alsa has been specified\n";
        exit(-1);
    }

    /* ============================== ALSA ============================== */
    if( m2i::use_alsa ){
    #ifdef WITH_ALSA
        m2i::alsa.init();
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

    /* =========================== Main Loop ============================ */
    LOG( INFO ) << "Main: Entering sleep, waiting for events\n";
    lua_getglobal( L, "pre_loop" );
    lua_pcall( L, 0, 0, 0 );
    lua_pop( L, 1);
    std::chrono::system_clock::time_point loop_last = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point watch_last = std::chrono::system_clock::now();
    while(! m2i::quit )
    {
        auto main_start = std::chrono::system_clock::now();

        #ifdef WITH_ALSA
        if( m2i::alsa.valid ){
            while( m2i::alsa.event_pending() > 0 ){
                m2i::lua_midirecv( m2i::L, m2i::alsa.event_receive() );
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


