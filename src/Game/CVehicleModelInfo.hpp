#pragma once

#include "Game/CBaseModelInfo.hpp"
#include "Game/VehicleType.hpp"

#include <cstdint>

class CVehicleModelInfo : public CBaseModelInfo
{
public:
	enum Flags
	{
		FLAG_SMALL_WORKER = 0,
		FLAG_BIG = 1,
		FLAG_PEDS_CAN_STAND_ON_TOP = 28,
	};

	struct FlagsBitset
	{
		uint8_t data[32]{};

		[[nodiscard]] bool get(int bit) const noexcept
		{
			return (data[bit / 8] >> (bit % 8)) & 1;
		}
	};

	VehicleType vehicle_type;
	FlagsBitset flags;
};
