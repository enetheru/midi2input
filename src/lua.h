
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

//TODO hijack the lua print function and redefine it in terms of my logging class

namespace m2i {
/* maybe use a class like below for easier lua management
class Lua {
public:
    Lua(){ init(); }
    ~Lua(){ fina(); }
    void init(){ L = luaL_newstate(); }
    void fina(){ lua_close( L ); }
    lua_state *operator()(){ return L; } 
private:
    lua_State *L;
};
 */

    lua_State *lua_init_new();

    int lua_loadscript( lua_State *L, const fs::path &script );
    int lua_midirecv( lua_State *L, const midi_event &event );

    // main function biding
    int lua_midisend( lua_State *L );
    int lua_exec( lua_State *L );
    int lua_quit( lua_State *L );

#ifdef WITH_XORG
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
#endif//WITH_XORG

#ifdef WITH_ALSA
    // ALSA function binding
    int lua_alsaconnect( lua_State *L );
#endif//WITH_ALSA

    // Jack function binding
    // for now its unexpceted that i will have anything here, controlling the midi
    //TODO int lua_jackconnect( lua_State *L );
    // subsystems individually doesnt seem relevant at this stage


}//end namespace m2i
#endif//M2I_LUA_H
