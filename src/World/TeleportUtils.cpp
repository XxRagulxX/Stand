#include "TeleportUtils.hpp"

#include "World/Self.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand::Features::TeleportHelpers
{
	TeleportPlace MakePlace(const char* name, float x, float y, float z, float heading)
	{
		return TeleportPlace{name, {x, y, z}, heading};
	}

	bool IsValidCoordinate(const rage::fvector3& pos)
	{
		return !(pos.x == 0.f && pos.y == 0.f && pos.z == 0.f);
	}

	bool IsPlayerInInterior()
	{
		int interior = INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID());
		return INTERIOR::IS_VALID_INTERIOR(interior);
	}

	static void PerformTeleport(const TeleportPlace place)
	{
		auto ped = Self::GetPed();
		if (!ped || !ped.IsValid())
			return;

		int handle = ped.GetHandle();

		if (auto veh = Self::GetVehicle())
			handle = veh.GetHandle();

		STREAMING::REQUEST_COLLISION_AT_COORD(place.position.x, place.position.y, place.position.z);

		STREAMING::NEW_LOAD_SCENE_START(
		    place.position.x, place.position.y, place.position.z,
		    place.position.x, place.position.y, place.position.z,
		    100.f, 0
		);

		int timeout = 0;
		while (!ENTITY::HAS_COLLISION_LOADED_AROUND_ENTITY(handle) && timeout < 100)
		{
			BUILTIN::WAIT(10);
			timeout++;
		}

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(
		    handle,
		    place.position.x,
		    place.position.y,
		    place.position.z,
		    false, false, false
		);

		ENTITY::SET_ENTITY_HEADING(handle, place.heading);

		STREAMING::NEW_LOAD_SCENE_STOP();
	}

	void TeleportEntityTo(const TeleportPlace& place)
	{
		if (!IsValidCoordinate(place.position))
			return;

		FiberPool::queueJob([place] {
			PerformTeleport(place);
		});
	}
}
