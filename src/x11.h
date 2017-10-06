#ifndef X11_H
#define X11_H

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>

extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}

int XErrorCatcher( Display *disp, XErrorEvent *xe );
Window XGetTopLevelParent( Display *xdp, Window w );
void detect_window();
int initialise( lua_State *L );

// functions to pass to lua
int lua_keypress( lua_State *L );
int lua_keydown( lua_State *L );
int lua_keyup( lua_State *L );
int lua_buttonpress( lua_State *L );
int lua_buttondown( lua_State *L );
int lua_buttonup( lua_State *L );
int lua_mousemove( lua_State *L );
int lua_mousepos( lua_State *L );

#endif //X11_H
