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

//local includes
#include "argh.h"
#include "log.h"
#include "cache_data.h"
#include "lua.h"
#include "midi.h"

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
    bool verbose = false;
    bool quiet = false;
    bool use_alsa = true;
    bool use_jack = false;
    bool use_xorg = true;
    bool reconnect = true;
    bool loop_enabled = false;
    std::chrono::milliseconds main_freq( 25 );
    std::chrono::milliseconds loop_freq( 100 );
    std::chrono::milliseconds watch_freq( 3000 );
    fs::path config( "config.lua" );
    fs::path script( "script.lua" );

    //program state
    lua_State *L;
    bool quit = false;
    #ifdef WITH_ALSA
    alsa_singleton *alsa = nullptr;
    #endif//WITH_ALSA

    #ifdef WITH_JACK
    jack_singleton *jack = nullptr;
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

int32_t
processEvent( const midi_event &event )
{
    auto L = m2i::L;
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
    m2i::L = luaL_newstate();
    auto L = m2i::L;
    luaL_openlibs( L );

    // Load configuraton lua script
    cmdl({"-c", "--config"}) >> m2i::config;
    if( loadLua( L, m2i::config ) != ECODE::SUCCESS ){
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
        m2i::verbose = true;

    if( cmdl[{ "-q", "--quiet" }] )
        m2i::quiet = true;

    if( cmdl[{ "-a", "--alsa" }] )
        m2i::use_alsa = true;

    if( cmdl[{ "-j", "--jack" }] )
        m2i::use_jack = true;

    if( !cmdl({"-s", "--script"}).str().empty() )
        cmdl({"-s", "--script"}) >> m2i::script;
    if( loadLua( L, m2i::script ) != ECODE::SUCCESS ){
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
        m2i::alsa = alsa_singleton::getInstance( true );
        if( m2i::alsa->valid )
            m2i::alsa->set_eventProcessor( processEvent );
    #else
        LOG( ERROR ) << "Not compiled with ALSA midi backend\n";
        exit(-1);
    #endif
    }

    /* ============================== Jack ============================== */
    if( m2i::use_jack ){
    #ifdef WITH_JACK
        m2i::jack = jack_singleton::getInstance( true );
        if( m2i::jack->valid )
            m2i::jack->set_eventProcessor( processEvent );
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
    if( initialise( m2i::L ) == ECODE::FAILURE ) exit(-1);
    #endif


    /* =========================== Main Loop ============================ */
    LOG( INFO ) << "Main: Entering sleep, waiting for events\n";
    std::chrono::system_clock::time_point loop_last = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point watch_last = std::chrono::system_clock::now();
    while(! m2i::quit )
    {
        auto main_start = std::chrono::system_clock::now();
        #ifdef WITH_XORG
        detect_window();
        #endif//WITH_XORG

        #ifdef WITH_ALSA
        if( m2i::alsa ) if( m2i::alsa->valid )
            m2i::alsa->midi_recv();
        #endif//WITH_ALSA

        #ifdef WITH_JACK
        //TODO have a look as to whether we can put jack in here, instead of its
        //own thing. or alternatively, find out how jack does it and copy them.
        #endif//WITH_JACK

        // run regular checks at watch_freq
        auto watch_wait = std::chrono::system_clock::now() - watch_last;
        if( watch_wait > m2i::watch_freq ){
            watch_last = std::chrono::system_clock::now();
            //TODO inotify to monitor and reload configuration
            //TODO update cache for connected ports
        }

        // run the loop lua function at loop_freq
        auto loop_wait = std::chrono::system_clock::now() - loop_last;
        if( m2i::loop_enabled && loop_wait > m2i::loop_freq ){
            loop_last = std::chrono::system_clock::now();
            lua_getglobal( L, "loop" );
            if( lua_pcall( L, 1, 0, 0 ) != 0 ) m2i::loop_enabled = false;
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


