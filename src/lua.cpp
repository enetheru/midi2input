#include <spdlog/spdlog.h>

#include "lua.h"
#include "midi.h"
#include "util.h"
#include "uinput.h"


#ifdef WITH_ALSA
#include "alsa.h"
#endif//WITH_ALSA

#ifdef WITH_JACK
#include "jack.h"
#endif//WITH_JACK

#ifdef WITH_XORG
#include "x11.h"
#endif//WITH_XORG

#include <string>
#include <chrono>

namespace m2i {
    extern Uinput uinput;

    #ifdef WITH_ALSA
    extern AlsaSeq seq;
    #endif//WITH_ALSA

    #ifdef WITH_JACK
    extern JackSeq jack;
    #endif//WITH_JACK

    extern bool quit;
    extern bool loop_enabled;

static const struct luaL_Reg funcs [] = {
  {"print",        lua_print},
  {"midi_send",    lua_midisend},
  {"exec",         lua_exec},
  {"quit",         lua_quit},
  {"loop_enabled", lua_loopenable},
  {"milliseconds", lua_milliseconds },
  {"keypress",     lua_keypress },
  {"keydown",      lua_keydown },
  {"keyup",        lua_keyup },
  {"mousemove",    lua_mousemove },
  {"mousewarp",    lua_mousewarp },
  {"mousescroll",  lua_mousescroll },
  {"mousehscroll",  lua_mousehscroll },

#ifdef WITH_XORG
  {"detectwindow", lua_detectwindow },
#endif//WITH_XORG
#ifdef WITH_ALSA
  {"alsaconnect",  lua_alsaconnect },
#endif//WITH_ALSA
#ifdef WITH_JACK
  {"jackconnect",  lua_jackconnect },
#endif//WITH_JACK
  {nullptr, nullptr} /* end of array */
};

lua_State *
register_lua_funcs(lua_State *L ){

    lua_getglobal(L, "_G");
    luaL_setfuncs(L, funcs, 0);
    lua_pop(L, 1);

    return L;
}

int
midi_to_lua(lua_State *L, const midi_event &event )
{
    lua_getglobal( L, "midi_recv" );
    lua_pushnumber( L, event.status );
    lua_pushnumber( L, event.data1 );
    lua_pushnumber( L, event.data2 );
    if( lua_pcall( L, 3, 0, 0 ) != LUA_OK ) {
        spdlog::error(FMT_STRING("LUA: call to function 'midi_recv' failed {}"), lua_tostring(L, -1));
        lua_pop( L, 1 );
    }
    return 0;
}

/* ========================= Lua Functions =========================== */
int
lua_print( lua_State* L ){
    std::stringstream output;

    int args = lua_gettop( L );
    for( int i = 1; i <= args; ++i ){
        output << lua_tostring(L, i);
    }
    spdlog::info( FMT_STRING( "SCRIPT: {}" ), output.str() );
    return 0;
}

int
lua_midisend( lua_State *L )
{
    midi_event event{};
    lua_pushnumber( L, 1 );
    lua_gettable( L, -2 );
    event.status =  static_cast<unsigned char>( luaL_checknumber( L, -1 ) );
    lua_pop( L, 1 );

    lua_pushnumber( L, 2 );
    lua_gettable( L, -2 );
    event.data1 =  static_cast<unsigned char>( luaL_checknumber( L, -1 ) );
    lua_pop( L, 1 );

    lua_pushnumber( L, 3 );
    lua_gettable( L, -2 );
    event.data2 =  static_cast<unsigned char>( luaL_checknumber( L, -1 ) );
    lua_pop( L, 1 );

    #ifdef WITH_ALSA
    if( m2i::seq )m2i::seq.event_send( event );
    #endif

    #ifdef WITH_JACK
    if( m2i::jack.valid )m2i::jack.event_send( event );
    #endif
    return 0;
}

int
lua_exec( lua_State *L )
{
    std::string command;
    command = luaL_checkstring( L, -1 );
    spdlog::info( FMT_STRING( "LUA: exec: {}" ), command );

    FILE *in;
    char buff[512];
    if(! (in = popen( command.c_str(), "r" ))){
        return 1;
    }
    while( fgets(buff, sizeof(buff), in) != nullptr ){
        // strip trailling newline
        size_t bufflen = strnlen(buff, 512);
        if( bufflen > 0 && buff[bufflen - 1] == '\n' ){
            buff[bufflen - 1] = '\0';
        }

        spdlog::info( FMT_STRING( "LUA: {}" ), buff );
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

int
lua_loopenable( lua_State *L )
{
    (void)L; //shutup unused variable;
    m2i::loop_enabled = true;
    return 0;
}

int
lua_milliseconds( lua_State *L )
{
    static auto first = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - first).count();
    lua_pushnumber( L, duration );
    return 1;
}

/* ==================== sending events to uinput ====================*/
int
lua_keypress( lua_State *L )
{
    auto input_event_code = static_cast<int>( luaL_checknumber( L, 1 ) );
    uinput.keypress( input_event_code );
    spdlog::info( FMT_STRING( "LUA: keypress: {}" ), input_event_code );
    return 0;
}

int
lua_keydown( lua_State *L )
{
    auto input_event_code = static_cast<int>( luaL_checknumber( L, 1 ) );
    uinput.keydown( input_event_code );
    spdlog::info( FMT_STRING( "LUA: keydown: {}" ), input_event_code );
    return 0;
}

int
lua_keyup( lua_State *L )
{
    auto input_event_code = static_cast<int>( luaL_checknumber( L, 1 ) );
    uinput.keyup( input_event_code );
    spdlog::info( FMT_STRING( "LUA: keyup: {}" ), input_event_code );
    return 0;
}

int
lua_mousemove( lua_State *L )
{
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );
    uinput.mousemove( x, y );

    spdlog::info( FMT_STRING( "LUA: mousemove: {},{}" ), x, y );
    return 0;
}

int
lua_mousewarp(lua_State *L )
{
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );
    uinput.mousewarp( x,y );

    spdlog::info( FMT_STRING( "LUA: mousewarp: {},{}" ), x, y );
    return 0;
}

int
lua_mousescroll( lua_State *L )
{
    auto distance = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    uinput.mousescroll( distance );
    spdlog::info( FMT_STRING( "LUA: mousescroll: {}" ), distance );
    return 0;
}

int
lua_mousehscroll( lua_State *L )
{
    auto distance = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    uinput.mousehscroll( distance );
    spdlog::info( FMT_STRING( "LUA: mousehscroll: {}" ), distance );
    return 0;
}

#ifdef WITH_XORG  /* ===================== X11 functions =========================== */
int
lua_detectwindow( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    std::string windowname = m2i::XDetectWindow( xdp );
    XCloseDisplay( xdp );

    if( windowname.empty() )
        return 0;
    lua_pushstring( L ,  windowname.c_str() );
    lua_setglobal( L, "WM_CLASS" );
    return 0;
}
#endif//WITHXORG

#ifdef WITH_ALSA
    /* ==================== ALSA Lua Bindings =========================== */
int
lua_alsaconnect( lua_State *L )
{
    /* the idea with this function is that it takes two named ports, or port id's
     * and connects them together, so the function takes two arguments, client
     * and port */
    std::string client = luaL_checkstring(L, 1);
    std::string port = luaL_checkstring(L, 2);
    m2i::seq.connect( client, port );
    return 0;
}
#endif//WITH_ALSA

#ifdef WITH_JACK
    /* ==================== Jack Lua Bindings =========================== */
int
lua_jackconnect( lua_State *L )
{
    (void)L;
    //TODO connect to jack port
    spdlog::warn( FMT_STRING( "LUA: This function is not implemented yet" ) );
    return 0;
}
#endif//WITH_JACK
}// end namespace m2i
