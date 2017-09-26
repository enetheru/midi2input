#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <sstream>
#include <cstring>

#include <unistd.h>

extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

#ifdef WITH_JACK
    #include "jack.h"
#endif

#ifdef WITH_ALSA
    #include "alsa.h"
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>

#include "argh.h"
#include "log.h"

namespace midi2input {
    lua_State *L;
    Display* xdp;
}

const char *helptext =
"USAGE: ./midi2input [options]"
"GENERAL OPTIONS:"
"   -h  --help      Print usage and exit"
"   -v  --verbose   Output more information"
"   -c  --config    SpeSpecify config file, default = ~/.config/midi2input.lua";


static int
lua_keypress( lua_State *L )
{
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( midi2input::xdp, keysym );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 1, CurrentTime );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 0, CurrentTime );
    LOG(INFO) << "keypress: " << XKeysymToString( keysym );
    return 0;
}

static int
lua_keydown( lua_State *L )
{
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( midi2input::xdp, keysym );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 1, CurrentTime );
    LOG(INFO) << "keydown: " << XKeysymToString( keysym );
    return 0;
}

static int
lua_keyup( lua_State *L )
{
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( midi2input::xdp, keysym );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 0, CurrentTime );
    LOG(INFO) << "keyup: " << XKeysymToString( keysym );
    return 0;
}

static int
lua_buttonpress( lua_State *L )
{
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( midi2input::xdp, button, 1, CurrentTime );
    XTestFakeButtonEvent( midi2input::xdp, button, 0, CurrentTime );
    LOG(INFO) << "buttonpress: " << button;
    return 0;
}

static int
lua_buttondown( lua_State *L )
{
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( midi2input::xdp, button, 1, CurrentTime );
    LOG(INFO) << "buttondown: " << button;
    return 0;
}

static int
lua_buttonup( lua_State *L )
{
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( midi2input::xdp, button, 0, CurrentTime );
    LOG(INFO) << "buttonup: " << button;
    return 0;
}

static int
lua_mousemove( lua_State *L )
{
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );
    XTestFakeRelativeMotionEvent( midi2input::xdp, x, y, CurrentTime );
    LOG(INFO) << "mousemove: " << x << "," << y;
    return 0;
}

static int
lua_mousepos( lua_State *L )
{
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );;
    XTestFakeMotionEvent( midi2input::xdp, -1, x, y, CurrentTime );
    LOG(INFO) << "mousewarp: " << x << "," << y;
    return 0;
}

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
    auto &alsa = alsa_singleton::getInstance();
    alsa.midi_send( event );
#endif

#ifdef WITH_JACK
    auto &jack = jack_singleton::getInstance();
    jack.midi_send( event );
#endif
    return 0;
}

static int
lua_exec( lua_State *L )
{
    std::string command;
    command = luaL_checkstring( L, -1 );
    LOG( INFO ) << "exec: " << command;

    FILE *in;
    char buff[512];
    if(! (in = popen( command.c_str(), "r" ))){
        return 1;
    }
    while( fgets(buff, sizeof(buff), in) != nullptr ){
        LOG( INFO ) << buff;
    }
    pclose( in );
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
        LOG( ERROR ) << "cannot run configuration file: " << lua_tostring( L, -1 );
        return false;
    }
    LOG( INFO ) << "Using: " << paths.front();
    return true;
}

int XErrorCatcher( Display *disp, XErrorEvent *xe )
{
    printf( "Something had happened, bruh.\n" );
    return 0;
}

Window
XGetTopLevelParent( Display *xdp, Window w )
{
    auto L = midi2input::L;

    LOG( ERROR ) << "Failed XInternAtom for WM_CLASS";
    Atom property = XInternAtom( xdp, "WM_CLASS", False );
    if( property == None ){
    }

    Atom actual_type_return;
    int actual_format_return = 0;
    unsigned long nitems_return = 0L;
    unsigned long bytes_after_return = 0L;
    unsigned char *prop_return = nullptr;

    if( XGetWindowProperty( xdp, w, property, 0L, 1024L, False, XA_STRING,
                &actual_type_return,
                &actual_format_return,
                &nitems_return,
                &bytes_after_return,
                &prop_return ) != Success ){
        LOG( ERROR ) << "Failed XGetWindowProperty for WM_CLASS";
    }
    if( (actual_type_return == XA_STRING) && (actual_format_return == 8) ){
        LOG( INFO ) << "WM_CLASS: " << prop_return;

        lua_pushstring( L , reinterpret_cast<const char *>(prop_return) );
        lua_setglobal( L, "wm_class" );

        XFree( prop_return );
        return w;
    }

    // no WM_CLASS property found, so lets get the parent window
    Window root_return;
    Window parent_return;
    Window *children_return = nullptr;
    unsigned int nchildren_return = 0;

    if( XQueryTree( xdp, w,
                &root_return,
                &parent_return,
                &children_return,
                &nchildren_return ) ){
        if( children_return != nullptr ) XFree( children_return );
        if( parent_return != DefaultRootWindow( xdp ) )
            w = XGetTopLevelParent( xdp, parent_return );
    }
    else {
        LOG( ERROR ) << "Failed XQueryTree";
    }

    return w;
}

int32_t
processEvent( const midi_event &event )
{
    auto L = midi2input::L;

    //FIXME this happens for every event, rather than intermittantly
    //which causes Xorg to take up 70% of my cpu when i'm pulling
    //midi events from a dsp envelope tracker and feeding it back.
    //i would rather it not do this.
    //detect window
    static Window w_current;
    Window w;
    int revert_to;

    XGetInputFocus( midi2input::xdp, &w, &revert_to );
    if( w != None && w != w_current ){
        w_current = w;
        XGetTopLevelParent( midi2input::xdp, w );
    }

    // TODO use inotify to monitor the state of the configuration file and
    // reload if it has changed

    lua_getglobal( L, "midi_recv" );
    lua_pushnumber( L, event[0] );
    lua_pushnumber( L, event[1] );
    lua_pushnumber( L, event[2] );
    if( lua_pcall( L, 3, 0, 0 ) != 0 )
        LOG( ERROR ) << "call to function 'event_in' failed" << lua_tostring( L, -1 );

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
        LOG(INFO) << helptext;
        exit( 0 );
    }

    // --config
    LOG( INFO ) << "Parsing cmd line options";
    std::string luaScript;
    if( cmdl[{"-c", "--config"}] )
    {
        luaScript = "";
    } else luaScript = "~/.config/midi2input.lua";

    /* X11 */
    LOG( INFO ) << "Getting X11 Display";
    if(! (midi2input::xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
        LOG( FATAL ) << "Unable to open X display";
        exit( -1 );
    }
    // set XErrorHandler
    XSetErrorHandler( XErrorCatcher );

    /* Lua */
    LOG( INFO ) << "Initialising Lua";

    midi2input::L = luaL_newstate();
    auto L = midi2input::L;
    luaL_openlibs( L );

    LOG( INFO ) << "Lua: pushing c functions";
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

    lua_pushcfunction( L, lua_midi_send );
    lua_setglobal( L, "midi_send" );

    lua_pushcfunction( L, lua_exec );
    lua_setglobal( L, "exec" );

    LOG( INFO ) << "Lua: Loading configuration file";
    if(! load_config( luaScript ) ){
        LOG( FATAL ) << "Unable to open configuration file, expecting ~/.config/midi2input.lua, or -c switch.";
        exit( -1 );

    }
#ifdef WITH_ALSA
    /* ALSA */
    auto &alsa = alsa_singleton::getInstance();
    alsa.set_eventProcessor( processEvent );
#endif

#ifdef WITH_JACK
    /* Jack */
    auto &jack = jack_singleton::getInstance();
    jack.set_eventProcessor( processEvent );
#endif

    /* main loop */
    LOG( INFO ) << "Main: Entering sleep, waiting for jack events";
    while( true ){
#ifdef WITH_ALSA
        alsa.midi_recv();
#endif
        sleep( 1 );
        //fixme put in something to read alsa events here
        //fixme and maybe something else to read a global variable to know when to quit.
    }

    lua_close( L );
    exit( 0 );
}


