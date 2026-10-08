#pragma once

#include "Game/fwddecl.hpp"
#include "Game/typedecl.hpp"

namespace Stand
{
	struct TranscendentVehicle
	{
		inline static bool active = false;
		inline static AbstractEntity ent;

		static void saveFromPlayerState();
		static void save(AbstractEntity& veh);
		static void recover();
		static void removeEntity();
	};
}
