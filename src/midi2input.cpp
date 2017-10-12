#include <chrono>
#include <thread>
#include <signal.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#include "argh.h"
#include "log.h"
#include "cache_data.h"
#include "lua.h"
#include "midi.h"

#ifdef WITH_JACK
    #include "jack.h"
#endif
#ifdef WITH_ALSA
    #include "alsa.h"
#endif

#ifdef WITH_XORG
    #include "x11.h"
#endif

namespace midi2input {
    lua_State *L;
    bool quit = false;

    #ifdef WITH_ALSA
    alsa_singleton *alsa = nullptr;
    #endif

    #ifdef WITH_JACK
    jack_singleton *jack = nullptr;
    #endif
    //configuration with defaults
    bool use_alsa = true;
    bool use_jack = false;
    bool use_xorg = true;
    bool verbose = false;
    bool quiet = false;
    bool reconnect = true;
    std::chrono::milliseconds timer_resolution( 100 );
    fs::path config( "config.lua" );
    fs::path script( "script.lua" );
}

const char *helptext =
"USAGE: ./midi2input [options]\n"
"OPTIONS:\n"
"   -h  --help                  Print usage and exit\n"
"   -v  --verbose               Output more information\n"
"   -c  --config <config.lua>   Specify config file\n"
"   -s  --script <script.lua>   Specify script file\n"
"   -a  --alsa                  Use ALSA midi backend\n"
"   -j  --jack                  Use Jack midi backend\n";
//TODO implement
//"   -r  --reconnect             Reconnect to previousl connected ports, or\n"
//"                               on jack or alsa failure\n";
//"   -x  --xorg_XORG             use xorg subsystems\n";

void intHandler( int dummy ){
    (void)dummy;
    midi2input::quit = true;
}

int32_t
processEvent( const midi_event &event )
{
    auto L = midi2input::L;
    lua_getglobal( L, "midi_recv" );
    lua_pushnumber( L, event[0] );
    lua_pushnumber( L, event[1] );
    lua_pushnumber( L, event[2] );
    if( lua_pcall( L, 3, 0, 0 ) != 0 )
        LOG( ERROR ) << "call to function 'event_in' failed" << lua_tostring( L, -1 ) << "\n";
    return 0;
}

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
    midi2input::L = luaL_newstate();
    auto L = midi2input::L;
    luaL_openlibs( L );

    // Load configuraton lua script
    cmdl({"-c", "--config"}) >> midi2input::config;
    if( loadLua( L, midi2input::config ) != ECODE::SUCCESS ){
        LOG( ERROR ) << "unable to load config: " << midi2input::config << "\n";
        LOG( WARN ) << "using default configuration\n";
    } else {
        cacheSet( "config", midi2input::config );

        //pull configuration from config.lua
        lua_getglobal( L, "config" );
        if( lua_istable(L, -1 ) ){
            lua_pushnil(L);

            while( lua_next( L, -2 ) != 0 ){
                std::string var( lua_tostring( L, -2 ) );
                    if( var == "script"   ) midi2input::script   = lua_tostring( L, -1 );
                else if( var == "verbose"  ) midi2input::verbose  = lua_toboolean( L, -1 );
                else if( var == "quiet"    ) midi2input::quiet    = lua_toboolean( L, -1 );
                else if( var == "use_alsa" ) midi2input::use_alsa = lua_toboolean( L, -1 );
                else if( var == "use_jack" ) midi2input::use_jack = lua_toboolean( L, -1 );
                else if( var == "reconnect" ) midi2input::reconnect = lua_toboolean( L, -1 );
                else if( var == "timer_resolution" )
                    midi2input::timer_resolution = std::chrono::milliseconds( lua_tointeger( L, -1 ) );

                lua_pop(L, 1);
            }
        }
        else {
            LOG( ERROR ) << "No 'config' table found in lua file\n";
        }
    }

    lua_pushcfunction( L, lua_midi_send );
    lua_setglobal( L, "midi_send" );

    lua_pushcfunction( L, lua_exec );
    lua_setglobal( L, "exec" );

    lua_pushcfunction( L, lua_quit );
    lua_setglobal( L, "quit" );

    
    /* ============================= cmdl =============================== */
    //cmdl overrides
    if( cmdl[{ "-v", "--verbose" }] )
        midi2input::verbose = true;

    if( cmdl[{ "-q", "--quiet" }] )
        midi2input::quiet = true;

    if( cmdl[{ "-a", "--alsa" }] )
        midi2input::use_alsa = true;

    if( cmdl[{ "-j", "--jack" }] )
        midi2input::use_jack = true;

    if( !cmdl({"-s", "--script"}).str().empty() )
        cmdl({"-s", "--script"}) >> midi2input::script;
    if( loadLua( L, midi2input::script ) != ECODE::SUCCESS ){
        LOG( ERROR ) << "Unable to find script file:" << midi2input::script << "\n";
    } else {
        cacheSet( "script", midi2input::script );
    }

    //one final check before continuing
    if( !midi2input::use_alsa && !midi2input::use_jack ){
        LOG( ERROR ) << "neither jack nor alsa has been specified\n";
        exit(-1);
    }

    /* ============================== ALSA ============================== */
    if( midi2input::use_alsa ){
    #ifdef WITH_ALSA
        midi2input::alsa = alsa_singleton::getInstance( true );
        if( midi2input::alsa->valid )
            midi2input::alsa->set_eventProcessor( processEvent );
    #else
        LOG( ERROR ) << "Not compiled with ALSA midi backend\n";
        exit(-1);
    #endif
    }

    /* ============================== Jack ============================== */
    if( midi2input::use_jack ){
    #ifdef WITH_JACK
        midi2input::jack = jack_singleton::getInstance( true );
        if( midi2input::jack->valid )
            midi2input::jack->set_eventProcessor( processEvent );
        else{
            LOG( ERROR ) << "Unable to connect to jack\n";
            exit(-1);
        }
    #else
        LOG( ERROR ) << "Not compiled with Jack midi backend\n";
        exit(-1);
    #endif
    }

    /* ============================= X11 ================================ */
    #ifdef WITH_XORG
    if( initialise( midi2input::L ) == ECODE::FAILURE ) exit(-1);
    #endif


    /* =========================== Main Loop ============================ */
    LOG( INFO ) << "Main: Entering sleep, waiting for events\n";
    while(! midi2input::quit )
    {
        auto start_time = std::chrono::system_clock::now();
        #ifdef WITH_XORG
        detect_window();
        #endif

        #ifdef WITH_ALSA
        if( midi2input::alsa ) if( midi2input::alsa->valid )
            midi2input::alsa->midi_recv();
        #endif

        //TODO inotify to monitor and reload configuration
        //TODO update cache for connected ports

        // loop spin has fixed interval at the moment, and there may be multiple
        // requirements for faster and slower interfals depending on the task at
        // hand. FIXME
        //I want this value configurable, infact I would really like multiple frequencies for different things.
        //a tight event loop with timers, triggers, can spin off threads and coroutines.
        //whats the minimum i need? call a function after a certain time with there being a certain minimum resolution

        //minimum resolution of time
        auto end_time = std::chrono::system_clock::now();
        std::chrono::duration< double > seconds = end_time - start_time;
        if( seconds > midi2input::timer_resolution )
            LOG( WARN ) << "processing time( " << std::setprecision(2) << seconds.count() * 1000 << "ms ) is longer than timer resolution\n";
        else
            std::this_thread::sleep_for( midi2input::timer_resolution - seconds );
    }

    lua_close( L );
    exit( 0 );
}


