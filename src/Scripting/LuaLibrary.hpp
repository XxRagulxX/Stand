#pragma once
#include "lua.hpp"

namespace Stand
{
	class LuaLibrary
	{
	public:
		LuaLibrary();

		virtual void Register(lua_State* state) = 0;
	};
}