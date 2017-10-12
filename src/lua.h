
#ifndef M2I_LUA_H
#define M2I_LUA_H

extern "C" {
    #include <lua5.2/lua.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lualib.h>
}
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "log.h"
#include "midi.h"

fs::path getPath( const fs::path &path );
ECODE loadLua( lua_State *L, const fs::path &script );

void processEvent( lua_State *L, const midi_event &event );

// functions to register in the lua environment
int lua_midi_send( lua_State *L );
int lua_exec( lua_State *L );
int lua_quit( lua_State *L );

#endif//M2I_LUA_H
