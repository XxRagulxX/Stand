#include "Scripting/LuaLibrary.hpp"
#include "Scripting/LuaMainUtils.hpp"
#include "Util/Joaat.hpp"

#include <chrono>

namespace Stand::Lua
{
	class Util : LuaLibrary
	{
		using LuaLibrary::LuaLibrary;

		static int Joaat(lua_State* state)
		{
			const char* string = CheckStringSafe(state, 1);
			lua_pushinteger(state, (int)Stand::Joaat(string));
			return 1;
		}

		static int Time(lua_State* state)
		{
			auto now = std::chrono::system_clock::now();
			auto duration = now.time_since_epoch();
			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

			lua_pushinteger(state, milliseconds);

			return 1;
		}

		virtual void Register(lua_State* state) override	
		{
			lua_newtable(state);
			SetFunction(state, Joaat, "joaat");
			SetFunction(state, Time, "time");
			lua_setglobal(state, "util");
		}
	};

	Util _Util;
}