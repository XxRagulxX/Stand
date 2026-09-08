#include "Scripting/LuaLibrary.hpp"
#include "Scripting/LuaMainUtils.hpp"
#include "World/Stats.hpp"

namespace Stand::Lua
{
	class Stats : LuaLibrary
	{
		using LuaLibrary::LuaLibrary;

		static int SetInt(lua_State* state)
		{
			Stand::Stats::SetInt(CheckStringSafe(state, 1), luaL_checkinteger(state, 2));
			return 0;
		}

		static int SetBool(lua_State* state)
		{
			Stand::Stats::SetBool(CheckStringSafe(state, 1), CheckBooleanSafe(state, 2));
			return 0;
		}

		static int SetFloat(lua_State* state)
		{
			Stand::Stats::SetFloat(CheckStringSafe(state, 1), luaL_checknumber(state, 2));
			return 0;
		}

		// SetDate

		static int SetString(lua_State* state)
		{
			Stand::Stats::SetString(CheckStringSafe(state, 1), CheckStringSafe(state, 2));
			return 0;
		}

		static int SetPackedInt(lua_State* state)
		{
			Stand::Stats::SetPackedInt(luaL_checkinteger(state, 1), luaL_checkinteger(state, 2));
			return 0;
		}

		static int SetPackedBool(lua_State* state)
		{
			Stand::Stats::SetPackedBool(luaL_checkinteger(state, 1), CheckBooleanSafe(state, 2));
			return 0;
		}

		static int SetPackedBoolRange(lua_State* state)
		{
			int start = luaL_checkinteger(state, 1);
			int end = luaL_checkinteger(state, 2);
			bool value = CheckBooleanSafe(state, 3);
			for (int i = start; i <= end; i++)
				Stand::Stats::SetPackedBool(i, value);
			return 0;
		}

		static int SetMaskedInt(lua_State* state)
		{
			Stand::Stats::SetMaskedInt(CheckStringSafe(state, 1), luaL_checkinteger(state, 2), luaL_checkinteger(state, 3), luaL_checkinteger(state, 4));
			return 0;
		}

		static int SetMaskedBool(lua_State* state)
		{
			Stand::Stats::SetMaskedBool(CheckStringSafe(state, 1), luaL_checkinteger(state, 2), CheckBooleanSafe(state, 3));
			return 0;
		}

		static int GetInt(lua_State* state)
		{
			lua_pushinteger(state, Stand::Stats::GetInt(CheckStringSafe(state, 1)));
			return 1;
		}

		static int GetBool(lua_State* state)
		{
			lua_pushboolean(state, Stand::Stats::GetBool(CheckStringSafe(state, 1)));
			return 1;
		}

		static int GetFloat(lua_State* state)
		{
			lua_pushnumber(state, Stand::Stats::GetFloat(CheckStringSafe(state, 1)));
			return 1;
		}

		// GetDate

		static int GetString(lua_State* state)
		{
			lua_pushstring(state, Stand::Stats::GetString(CheckStringSafe(state, 1)));
			return 1;
		}

		static int GetPackedInt(lua_State* state)
		{
			lua_pushinteger(state, Stand::Stats::GetPackedInt(luaL_checkinteger(state, 1)));
			return 1;
		}

		static int GetPackedBool(lua_State* state)
		{
			lua_pushboolean(state, Stand::Stats::GetPackedBool(luaL_checkinteger(state, 1)));
			return 1;
		}

		static int GetMaskedInt(lua_State* state)
		{
			lua_pushinteger(state, Stand::Stats::GetMaskedInt(CheckStringSafe(state, 1), luaL_checkinteger(state, 2), luaL_checkinteger(state, 3)));
			return 1;
		}

		static int GetMaskedBool(lua_State* state)
		{
			lua_pushboolean(state, Stand::Stats::GetMaskedBool(CheckStringSafe(state, 1), luaL_checkinteger(state, 2)));
			return 1;
		}

		virtual void Register(lua_State* state) override
		{
			lua_newtable(state);
			SetFunction(state, SetInt, "set_int");
			SetFunction(state, SetBool, "set_bool");
			SetFunction(state, SetFloat, "set_float");
			SetFunction(state, SetString, "set_string");
			SetFunction(state, SetPackedInt, "set_packed_int");
			SetFunction(state, SetPackedBool, "set_packed_bool");
			SetFunction(state, SetPackedBoolRange, "set_packed_bool_range");
			SetFunction(state, SetMaskedInt, "set_masked_int");
			SetFunction(state, SetMaskedBool, "set_masked_bool");
			SetFunction(state, GetInt, "get_int");
			SetFunction(state, GetBool, "get_bool");
			SetFunction(state, GetFloat, "get_float");
			SetFunction(state, GetString, "get_string");
			SetFunction(state, GetPackedInt, "get_packed_int");
			SetFunction(state, GetPackedBool, "get_packed_bool");
			SetFunction(state, GetMaskedInt, "get_masked_int");
			SetFunction(state, GetMaskedBool, "get_masked_bool");
			lua_setglobal(state, "stats");
		}
	};

	Stats _Stats;
}