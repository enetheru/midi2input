#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <sstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
//FIXME create enumerations of error messages from functions rather than use the
//c convention of using integers with error numbers
enum class ECODE {
    SUCCESS,
    FAILURE,
};

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
    //configuration with defaults
    bool use_alsa = true;
    bool use_jack = false;
    bool verbose = false;
    bool quiet = false;
    fs::path config( "config.lua" );
    fs::path script( "script.lua" );
}

const char *helptext =
"USAGE: ./midi2input [options]"
"GENERAL OPTIONS:"
"   -h  --help      Print usage and exit"
"   -v  --verbose   Output more information"
"   -c  --config    Specify config file, default = ~/.config/midi2input/config.lua"
"   -s  --script    Specify script file, default = ~/.config/midi2input/script.lua"
"   -a  --alsa      Use ALSA midi backend"
"   -j  --jack      Use Jack midi backend";

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

fs::path
getPath( const fs::path &path )
{
    if( fs::exists( path ) && fs::is_regular_file( path ) )
        return fs::absolute( path );

    if( path.is_absolute() ) return fs::path();

    fs::path temp;
    if( getenv( "XDG_CONFIG_HOME" ) )
        temp = std::string( getenv( "XDG_CONFIG_HOME") ) + "/midi2input/" + path.string();
    else
        temp = std::string( getenv( "HOME") ) + "/.config/midi2input/" + path.string();

    if( fs::exists( temp ) && fs::is_regular_file( temp ) ){
        return fs::absolute( temp );
    }
    return fs::path();
}

ECODE
loadScript( lua_State *L, const fs::path &script )
{
    fs::path path = getPath( script );
    if( path.empty() ) return ECODE::FAILURE;

    LOG( INFO ) << "Loading script: " << path << "\n";

    if( luaL_loadfile( L, path.c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
        LOG( ERROR ) << "cannot execute script file: " << lua_tostring( L, -1 ) << "\n";
        return ECODE::FAILURE;
    }
    return ECODE::SUCCESS;
}

ECODE
loadConfig( lua_State *L, const fs::path &config )
{
    fs::path path = getPath( config );
    if( path.empty() ) return ECODE::FAILURE;

    LOG( INFO ) << "Loading config: " << path << "\n";
    if( luaL_loadfile( L, path.c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
        LOG( ERROR ) << "cannot load config file: " << lua_tostring( L, -1 ) << "\n";
        return ECODE::FAILURE;
    }
    //else
    cacheSet( "config", path );

    lua_getglobal( L, "config" );
    lua_pushnil(L);

    while(lua_next(L, -2) != 0)
    {
        std::string var( lua_tostring( L, -2 ) );
                if( var == "script"   ) midi2input::script   = lua_tostring( L, -1 );
        else if( var == "verbose"  ) midi2input::verbose  = lua_toboolean( L, -1 );
        else if( var == "quiet"    ) midi2input::quiet    = lua_toboolean( L, -1 );
        else if( var == "use_alsa" ) midi2input::use_alsa = lua_toboolean( L, -1 );
        else if( var == "use_jack" ) midi2input::use_jack = lua_toboolean( L, -1 );

        lua_pop(L, 1);
    }
    return ECODE::SUCCESS;
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
    argh::parser cmdl( { "-c", "--config", "-s", "--script" } );
    cmdl.parse( argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION );

    if( cmdl[{"-h", "--help"}] ){
        LOG(INFO) << helptext << "\n";
        exit( 0 );
    }

    //load lua environment first
    midi2input::L = luaL_newstate();
    auto L = midi2input::L;
    luaL_openlibs( L );

    // Load configuraton
    cmdl({"-c", "--config"}) >> midi2input::config;
    if( loadConfig( L, midi2input::config ) != ECODE::SUCCESS ){
        LOG( ERROR ) << "unable to load config: " << midi2input::config << "\n";
        LOG( WARN ) << "using default configuration\n";
    }

    //cmdl overrides
    if( cmdl[{ "-v", "--verbose" }] )
        midi2input::verbose = true;

    if( cmdl[{ "-q", "--quiet" }] )
        midi2input::quiet = true;

    if( cmdl[{ "-a", "--alsa" }] )
        midi2input::use_alsa = true;

    if( cmdl[{ "-j", "--jack" }] )
        midi2input::use_jack = true;

    if( !midi2input::use_alsa && !midi2input::use_jack ){
        LOG( ERROR ) << "neither jack nor alsa has been specified\n";
        exit(-1);
    }
    /* ============================== Lua =============================== */

    lua_pushcfunction( L, lua_midi_send );
    lua_setglobal( L, "midi_send" );

    lua_pushcfunction( L, lua_exec );
    lua_setglobal( L, "exec" );

    lua_pushcfunction( L, lua_quit );
    lua_setglobal( L, "quit" );

    if( !cmdl({"-s", "--script"}).str().empty() )
        cmdl({"-s", "--script"}) >> midi2input::script;
    if( loadScript( L, midi2input::script ) != ECODE::SUCCESS ){
        LOG( ERROR ) << "Unable to find script file:" << midi2input::script << "\n";
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
        //TODO update cache for connected ports

        // loop spin has fixed interval at the moment, and there may be multiple
        // requirements for faster and slower interfals depending on the task at
        // hand. FIXME
        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
    }

    lua_close( L );
    exit( 0 );
}


