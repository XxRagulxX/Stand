#include "Scripting/LuaLibrary.hpp"
#include "Scripting/LatentFunction.hpp"
#include "Vehicle/Vehicle.hpp"
#include "World/Self.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Lua
{
	// only use for internal testing
	class Internal : LuaLibrary
	{
		using LuaLibrary::LuaLibrary;

		static int SpawnVehicle(lua_State* state)
		{
			const char* model = luaL_checkstring(state, 1);
			Vehicle::Create(Joaat(model), Self::GetPed().GetPosition());
			return 0;
		}

		virtual void Register(lua_State* state) override
		{
			lua_newtable(state);

			lua_pushcfunction(state, LatentFunction<SpawnVehicle>);
			lua_setfield(state, -2, "spawn_vehicle");

			lua_setglobal(state, "internal");
		}
	};

	Internal _Internal;
}