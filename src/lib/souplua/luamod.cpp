// Soup Lua Bindings are entirely contained within the .hpp
// This file only serves for those who want to compile them to a Lua module.

#include <lua.h>
#include <lauxlib.h>

#include "souplua/soup_lua_bindings.hpp"

SOUP_CEXPORT int luaopen_soup(lua_State* L)
{
	soup::LuaBindings::open_pushTable(L);
	return 1;
}
