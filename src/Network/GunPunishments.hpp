#pragma once

#include "Game/typedecl.hpp"

#include "lib/soup/Bitset.hpp"

namespace Stand
{
	struct GunPunishments
	{
		inline static soup::Bitset<punishment_t> aim_punishments{};
		inline static soup::Bitset<punishment_t> shoot_punishments{};

		static void onTick();
	};
}
