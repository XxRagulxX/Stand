#pragma once

#include <cstdint>

namespace Stand
{
	struct PlayerExcludes
	{
		bool isExcluded(class AbstractPlayer) const { return false; }
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
