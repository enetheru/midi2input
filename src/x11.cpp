#include "log.h"
#include "x11.h"

namespace midi2input {
    Display *xdp = nullptr;
    extern lua_State *L;
}

int XErrorCatcher( Display *disp, XErrorEvent *xe )
{
    (void)disp; (void)xe;
    //FIXME this is to shut things up; we might want to
    // handle this at some point in the future though
    LOG( ERROR ) << "Something had happened, bruh\n";
    return 0;
}

Window
XGetParent( Display *xdp, Window w )
{
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
            return parent_return;
        else return -1;
    }
    else {
        LOG( ERROR ) << "Failed XQueryTree\n";
    }
    return -1;
}

void
detect_window()
{
    static Window w_current;
    auto L = midi2input::L;

    auto xdp = midi2input::xdp;
    if(! xdp ){
        LOG( ERROR ) << "invalid handle to X display\n";
        return;
    }

    Window w;
    int revert_to;
    XGetInputFocus( xdp, &w, &revert_to );
    if( w == None || w == w_current )return;
    else w_current = w;

    while( true ){
        Atom property = XInternAtom( xdp, "WM_CLASS", False );
        if( property == None ){
            LOG( ERROR ) << "Failed XInternAtom for WM_CLASS\n";
            break;
        }

        Atom actual_type_return;
        int actual_format_return = 0;
        unsigned long nitems_return = 0L;
        unsigned long bytes_after_return = 0L;
        unsigned char *prop_return = nullptr;

        if( XGetWindowProperty(
                    xdp, w, property, 0L, 1024L, False, XA_STRING,
                    &actual_type_return,
                    &actual_format_return,
                    &nitems_return,
                    &bytes_after_return,
                    &prop_return ) == Success )
        {
            if( actual_type_return == XA_STRING
             && actual_format_return == 8 ){
                LOG( INFO ) << "WM_CLASS: " << prop_return << "\n";

                lua_pushstring( L , reinterpret_cast< const char *>( prop_return ) );
                lua_setglobal( L, "wm_class" );

                XFree( prop_return );
                return;
            }
            if( actual_format_return == None )
                LOG( INFO ) << "property: WM_CLASS not present\n";
        }
        else{
            LOG( ERROR ) << "XGetWindowProperty did not return Success\n";
            return;
        }

        w = XGetParent( xdp, w );
    }
}

ECODE
initialise( lua_State* L )
{
    LOG( INFO ) << "Getting X11 Display\n";
    if(! (midi2input::xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
        LOG( FATAL ) << "Unable to open X display\n";
        return ECODE::FAILURE;
    }
    XSetErrorHandler( XErrorCatcher );

    LOG( INFO ) << "Lua: pushing X11 functions\n";
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

    return ECODE::SUCCESS;
}

int
lua_keypress( lua_State *L )
{
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( midi2input::xdp, keysym );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 1, CurrentTime );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 0, CurrentTime );
    LOG(INFO) << "keypress: " << XKeysymToString( keysym ) << "\n";
    return 0;
}

int
lua_keydown( lua_State *L )
{
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( midi2input::xdp, keysym );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 1, CurrentTime );
    LOG(INFO) << "keydown: " << XKeysymToString( keysym ) << "\n";
    return 0;
}

int
lua_keyup( lua_State *L )
{
    auto keysym = static_cast<KeySym>( luaL_checknumber( L, 1 ) );
    KeyCode keycode = XKeysymToKeycode( midi2input::xdp, keysym );
    XTestFakeKeyEvent( midi2input::xdp, keycode, 0, CurrentTime );
    LOG(INFO) << "keyup: " << XKeysymToString( keysym ) << "\n";
    return 0;
}

int
lua_buttonpress( lua_State *L )
{
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( midi2input::xdp, button, 1, CurrentTime );
    XTestFakeButtonEvent( midi2input::xdp, button, 0, CurrentTime );
    LOG(INFO) << "buttonpress: " << button << "\n";
    return 0;
}

int
lua_buttondown( lua_State *L )
{
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( midi2input::xdp, button, 1, CurrentTime );
    LOG(INFO) << "buttondown: " << button << "\n";
    return 0;
}

int
lua_buttonup( lua_State *L )
{
    auto button = static_cast<uint32_t>( luaL_checknumber( L, 1 ) );
    XTestFakeButtonEvent( midi2input::xdp, button, 0, CurrentTime );
    LOG(INFO) << "buttonup: " << button << "\n";
    return 0;
}

int
lua_mousemove( lua_State *L )
{
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );
    XTestFakeRelativeMotionEvent( midi2input::xdp, x, y, CurrentTime );
    LOG(INFO) << "mousemove: " << x << "," << y << "\n";
    return 0;
}

int
lua_mousepos( lua_State *L )
{
    auto x = static_cast<int32_t>( luaL_checknumber( L, 1 ) );
    auto y = static_cast<int32_t>( luaL_checknumber( L, 2 ) );;
    XTestFakeMotionEvent( midi2input::xdp, -1, x, y, CurrentTime );
    LOG(INFO) << "mousewarp: " << x << "," << y << "\n";
    return 0;
}
