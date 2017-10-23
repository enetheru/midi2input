#include "lua.h"
#include "midi.h"
#include "util.h"


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

//FIXME ZOMG this is so ugly.
namespace m2i {
    #ifdef WITH_ALSA
    extern snd::Seq seq;
    #endif//WITH_ALSA

    #ifdef WITH_JACK
    extern JackSeq jack;
    #endif//WITH_JACK

    extern bool quit;

lua_State *
lua_init_new()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs( L );

    // push main functions to lua
    lua_pushcfunction( L, lua_midisend );
    lua_setglobal( L, "midi_send" );

    lua_pushcfunction( L, lua_exec );
    lua_setglobal( L, "exec" );

    lua_pushcfunction( L, lua_quit );
    lua_setglobal( L, "quit" );

#ifdef WITH_XORG
    // push x11 functions to lua
    lua_pushcfunction( L, lua_keypress );
    lua_setglobal( L, "keypress" );

    lua_pushcfunction( L, lua_keydown );
    lua_setglobal( L, "keydown" );

    lua_pushcfunction( L, lua_keyup );
    lua_setglobal( L, "keyup" );

    lua_pushcfunction( L, lua_buttonpress );
    lua_setglobal( L, "buttonpress" );

    lua_pushcfunction( L, lua_buttondown );
    lua_setglobal( L, "buttondown" );

    lua_pushcfunction( L, lua_buttonup );
    lua_setglobal( L, "buttonup" );

    lua_pushcfunction( L, lua_mousemove );
    lua_setglobal( L, "mousemove" );

    lua_pushcfunction( L, lua_mousepos );
    lua_setglobal( L, "mousepos" );

    lua_pushcfunction( L, lua_detectwindow );
    lua_setglobal( L, "detectwindow" );
#endif//WITH_XORG

#ifdef WITH_ALSA
    lua_pushcfunction( L, lua_alsaconnect );
    lua_setglobal( L, "alsaconnect" );
#endif//WITH_ALSA

#ifdef WITH_JACK
    //TODO connect to port
#endif//WITH_JACK
    return L;
}

int
lua_loadscript( lua_State *L, const fs::path &path )
{
    if( path.empty() ) return -1;
    if( !L ){ LOG( ERROR ) << "lua isnt initialised\n"; return -1; } 

    LOG( INFO ) << "Loading script: " << path << "\n";
    if( luaL_loadfile( L, path.c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
        LOG( ERROR ) << "failure loading script file: " << lua_tostring( L, -1 ) << "\n";
        return -1;
    }
    return 0;
}

int
lua_midirecv( lua_State *L, const midi_event &event )
{
    lua_getglobal( L, "midi_recv" );
    lua_pushnumber( L, event.status );
    lua_pushnumber( L, event.data1 );
    lua_pushnumber( L, event.data2 );
    if( lua_pcall( L, 3, 0, 0 ) != 0 )
        LOG( ERROR ) << "call to function 'event_in' failed" << lua_tostring( L, -1 ) << "\n";
    return 0;
}

    /* ========================= Lua Bindings =========================== */
int
lua_midisend( lua_State *L )
{
    midi_event event;
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

#ifdef WITH_XORG
    /* ===================== X11 Lua Bindings =========================== */
int
lua_keypress( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( xdp, keysym );
    XTestFakeKeyEvent( xdp, keycode, 1, CurrentTime );
    XTestFakeKeyEvent( xdp, keycode, 0, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << "keypress: " << XKeysymToString( keysym ) << "\n";
    return 0;
}

int
lua_keydown( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    XTestFakeKeyEvent( xdp, XKeysymToKeycode( xdp, keysym ), 1, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << "keydown: " << XKeysymToString( keysym ) << "\n";
    return 0;
}

int
lua_keyup( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    XTestFakeKeyEvent( xdp, XKeysymToKeycode( xdp, keysym ), 0, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << "keyup: " << XKeysymToString( keysym ) << "\n";
    return 0;
}

int
lua_buttonpress( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) ); 
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( xdp, button, 1, CurrentTime );
    XTestFakeButtonEvent( xdp, button, 0, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << "buttonpress: " << button << "\n";
    return 0;
}

int
lua_buttondown( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( xdp, button, 1, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << "buttondown: " << button << "\n";
    return 0;
}

int
lua_buttonup( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( xdp, button, 0, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << "buttonup: " << button << "\n";
    return 0;
}

int
lua_mousemove( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );
    XTestFakeRelativeMotionEvent( xdp, x, y, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << fmt::format( "mousemove: {},{}\n", x, y );
    return 0;
}

int
lua_mousepos( lua_State *L )
{
    Display *xdp = XOpenDisplay( getenv( "DISPLAY" ) );
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );;
    XTestFakeMotionEvent( xdp, -1, x, y, CurrentTime );
    XCloseDisplay( xdp );

    LOG(INFO) << fmt::format( "mousewarp: {},{}\n", x, y );
    return 0;
}

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
//TODO connect to jack port
#endif//WITH_JACK
}// end namespace m2i
