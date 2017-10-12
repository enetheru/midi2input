#include "lua.h"
#include "midi.h"
#include "alsa.h"
#include "jack.h"
#include "log.h"

#include <string>

//FIXME ZOMG this is so ugly.
namespace m2i {
    #ifdef WITH_ALSA
    extern alsa_singleton *alsa;
    #endif//WITH_ALSA
    #ifdef WITH_JACK
    extern jack_singleton *jack;
    #endif//WITH_JACK

    extern bool quit;
}

fs::path
getPath( const fs::path &path )
{
    if( fs::exists( path ) && fs::is_regular_file( path ) )
        return fs::absolute( path );

    if( path.is_absolute() ) return fs::path();

    fs::path temp;
    if( getenv( "XDG_CONFIG_HOME" ) )
        temp = std::string( getenv( "XDG_CONFIG_HOME") ) + "/m2i/" + path.string();
    else
        temp = std::string( getenv( "HOME") ) + "/.config/m2i/" + path.string();

    if( fs::exists( temp ) && fs::is_regular_file( temp ) ){
        return fs::absolute( temp );
    }
    return fs::path();
}

ECODE
loadLua( lua_State *L, const fs::path &script )
{
    fs::path path = getPath( script );
    if( path.empty() ) return ECODE::FAILURE;

    LOG( INFO ) << "Loading config: " << path << "\n";
    if( luaL_loadfile( L, path.c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
        LOG( ERROR ) << "cannot load config file: " << lua_tostring( L, -1 ) << "\n";
        return ECODE::FAILURE;
    }
    return ECODE::SUCCESS;
}

void
processEvent( lua_State *L, const midi_event &event )
{
    lua_getglobal( L, "midi_recv" );
    lua_pushnumber( L, event[0] );
    lua_pushnumber( L, event[1] );
    lua_pushnumber( L, event[2] );
    if( lua_pcall( L, 3, 0, 0 ) != 0 )
        LOG( ERROR ) << "call to function 'event_in' failed" << lua_tostring( L, -1 ) << "\n";
}

int
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
    if( m2i::alsa )
        if( m2i::alsa->valid )
            m2i::alsa->midi_send( event );
    #endif

    #ifdef WITH_JACK
    if( m2i::jack )
        if( m2i::jack->valid )
            m2i::jack->midi_send( event );
    #endif
    return 0;
}

int
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

int
lua_quit( lua_State *L )
{
    (void)L; //shutup unused variable;
    m2i::quit = true;
    return 0;
}
