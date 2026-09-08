#pragma once
#include "Core/types.hpp"

namespace Stand
{
	class VehicleModel
	{
	public:
		static bool CheckModBlacklist(Hash model, int mod_slot, int mod);
		static const char* GetModSlotName(Hash model, int vehicle, int mod_slot);
		static std::string GetModName(Hash model, int vehicle, int mod_slot, int mod, int mod_count);
		static bool IsBennys(int veh);
	};
}
