#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <sstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <unistd.h>

extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#include "argh.h"
#include "log.h"
#include "cache_data.h"

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
}

const char *helptext =
"USAGE: ./midi2input [options]"
"GENERAL OPTIONS:"
"   -h  --help      Print usage and exit"
"   -v  --verbose   Output more information"
"   -c  --config    Specify config file, default = ~/.config/midi2input.lua"
"   -a  --alsa      Use ALSA midi backend "
"   -j  --jack      Use Jack midi backend ";

static int
lua_midi_send( lua_State *L )
{
    midi_event event;
    for( int i = 0; i < 3; ++i ){
        lua_pushnumber( L, i + 1 );
        lua_gettable( L, -2 );
        event[ i ] =  static_cast<unsigned char>( luaL_checknumber( L, -1 ) );
        lua_pop( L, 1 );
    }

    #ifdef WITH_ALSA
    if( midi2input::alsa )
        if( midi2input::alsa->valid )
            midi2input::alsa->midi_send( event );
    #endif

    #ifdef WITH_JACK
    if( midi2input::jack )
        if( midi2input::jack->valid )
            midi2input::jack->midi_send( event );
    #endif
    return 0;
}

static int
lua_exec( lua_State *L )
{
    std::string command;
    command = luaL_checkstring( L, -1 );
    LOG( INFO ) << "exec: " << command << "\n";

    FILE *in;
    char buff[512];
    if(! (in = popen( command.c_str(), "r" ))){
        return 1;
    }
    while( fgets(buff, sizeof(buff), in) != nullptr ){
        LOG( INFO ) << buff << "\n";
    }
    pclose( in );
    return 0;
}

static int
lua_quit( lua_State *L )
{
    (void)L; //shutup unused variable;
    midi2input::quit = true;
    return 0;
}

bool
load_config( const std::string &name )
{
    auto L = midi2input::L;
    // load configuration from a priority list of locations
    // * specified from the command line
    // * configuration folder $HOME/.config/
    // * home directory $HOME/
    std::string filepath;
    std::queue<std::string> paths;

    if(! name.empty() ) paths.push( name );

    // configuration folder ~/.config/
    filepath = std::string( getenv( "HOME" ) ) + "/.config/midi2input.lua";
    paths.push( filepath );

    // configuration folder ~/.config/
    filepath = std::string( getenv( "HOME" ) ) + "/.midi2input.lua";
    paths.push( filepath );

    std::ifstream tFile;
    while(! paths.empty() ){
        tFile.open( paths.front().c_str(), std::ifstream::in );
        if( tFile.is_open() ){
            tFile.close();
            break;
        }
        paths.pop();
    }
    if( paths.empty() ) return false;

    if( luaL_loadfile( L, paths.front().c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
        LOG( ERROR ) << "cannot run configuration file: " << lua_tostring( L, -1 ) << "\n";
        return false;
    }
    LOG( INFO ) << "Using: " << paths.front() << "\n";
    return true;
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
    argh::parser cmdl( argc, argv );

    // Options Parsing
    // ===============
    // setup logging level.
    if( cmdl[{ "-v", "--verbose" }] )
    //if( options[ QUIET ] )
    //    LOG::SetDefaultLoggerLevel( LOG::CHECK );

    if( cmdl[{"-h", "--help"}] ){
        LOG(INFO) << helptext << "\n";
        exit( 0 );
    }

    //FIXME dont quit here, instead check configuration first.
    if( !cmdl[{"-a","--alsa"}] && !cmdl[{"-j","--jack"}] ){
        LOG( ERROR ) << "neither jack nor alsa specified on the command line, whats the point in going on?\n";
        exit(-1);
    }
    /* ============================== Lua =============================== */
    // --config
    LOG( INFO ) << "Parsing cmd line options\n";
    std::string luaScript;
    if( cmdl[{"-c", "--config"}] )
    {
        luaScript = "";
    } else luaScript = "~/.config/midi2input.lua";

    LOG( INFO ) << "Initialising Lua\n";

    midi2input::L = luaL_newstate();
    auto L = midi2input::L;
    luaL_openlibs( L );

    lua_pushcfunction( L, lua_midi_send );
    lua_setglobal( L, "midi_send" );

    lua_pushcfunction( L, lua_exec );
    lua_setglobal( L, "exec" );

    lua_pushcfunction( L, lua_quit );
    lua_setglobal( L, "quit" );

    LOG( INFO ) << "Lua: Loading configuration file\n";
    if(! load_config( luaScript ) ){
        LOG( FATAL ) << "Unable to open configuration file, expecting ~/.config/midi2input.lua, or -c switch.\n";
        exit( -1 );
    }
    //TODO pull configuration from file before continuing.

    /* ============================== ALSA ============================== */
    if( cmdl[{"-a","--alsa"}] ){
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
    if( cmdl[{"-j","--jack"}] ){
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
    if( initialise( midi2input::L ) ) exit(-1);
    #endif


    /* =========================== Main Loop ============================ */
    LOG( INFO ) << "Main: Entering sleep, waiting for events\n";
    while(! midi2input::quit )
    {
        #ifdef WITH_XORG
        detect_window();
        #endif

        #ifdef WITH_ALSA
        if( midi2input::alsa ) if( midi2input::alsa->valid )
            midi2input::alsa->midi_recv();
        #endif

        //TODO something to know when to quit.
        //TODO inotify to monitor and reload configuration

        // loop spin has fixed interval at the moment, and there may be multiple
        // requirements for faster and slower interfals depending on the task at
        // hand. FIXME
        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
    }

    lua_close( L );
    exit( 0 );
}


