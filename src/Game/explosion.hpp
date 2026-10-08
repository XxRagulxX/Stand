#pragma once

#include <cstdint>

#include "Core/types.hpp"

class CExplosionManager
{
public:
	struct CExplosionArgs
	{
		Entity source_entity{};
		Entity target_entity{};
		Hash explosion_type{};
		float damage_scale{1.0f};
		bool is_audible{true};
		bool is_invisible{false};
		float camera_shake{0.0f};
		bool noDamage{false};
		bool bNetworkControlled{false};
	};
};
