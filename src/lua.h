
#ifndef M2I_LUA_H
#define M2I_LUA_H

extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "util.h"
#include "midi.h"

//TODO redefine print in terms of my logging class
namespace m2i {
    void lua_init_new( lua_State *L );

    int lua_loadscript( lua_State *L, const fs::path &script );
    int lua_midirecv( lua_State *L, const midi_event &event );

    // main function biding
    int lua_midisend( lua_State *L );
    int lua_exec( lua_State *L );
    int lua_quit( lua_State *L );

    // X11 function binding
    int lua_keypress( lua_State *L );
    int lua_keydown( lua_State *L );
    int lua_keyup( lua_State *L );
    int lua_buttonpress( lua_State *L );
    int lua_buttondown( lua_State *L );
    int lua_buttonup( lua_State *L );
    int lua_mousemove( lua_State *L );
    int lua_mousepos( lua_State *L );
    int lua_detectwindow( lua_State *L );

    // Jack function binding
    // for now its unexpceted that i will have anything here, controlling the midi
    // subsystems individually doesnt seem relevant at this stage

    // ALSA function binding
    // for now its unexpceted that i will have anything here, controlling the midi
    // subsystems individually doesnt seem relevant at this stage

}//end namespace m2i
#endif//M2I_LUA_H
