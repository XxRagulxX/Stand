#pragma once

#include <cstdint>
#include "Game/gta_player.hpp"

namespace Stand
{
	struct PlayerExcludes
	{
		bool isExcluded(compactplayer_t) const { return false; }
	};

	struct TargetingData
	{
		PlayerExcludes excludes;
		bool players = true;
		bool peds = true;
		bool vehicles = false;
		bool objects = false;
		bool use_range = false;
		uint16_t range = 1000;
	};
}
