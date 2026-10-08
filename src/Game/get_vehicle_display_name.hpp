#pragma once

#include <string>

#include "Game/CVehicleModelInfo.hpp"
#include "Game/VehicleItem.hpp"

namespace Stand
{
	[[nodiscard]] std::string get_vehicle_display_name_no_manufacturer(hash_t model);
	[[nodiscard]] std::string get_vehicle_display_name_no_manufacturer(const char* modelName);
	[[nodiscard]] std::string get_vehicle_display_name_no_manufacturer(const VehicleItem* veh);
	[[nodiscard]] std::string get_vehicle_display_name(const VehicleItem* veh);
	[[nodiscard]] std::string get_vehicle_display_name(const CVehicleModelInfo* veh);
}
