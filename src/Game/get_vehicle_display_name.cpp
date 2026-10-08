#include "Game/get_vehicle_display_name.hpp"

namespace Stand
{
	std::string get_vehicle_display_name_no_manufacturer(hash_t)
	{
		return {};
	}

	std::string get_vehicle_display_name_no_manufacturer(const char*)
	{
		return {};
	}

	std::string get_vehicle_display_name_no_manufacturer(const VehicleItem*)
	{
		return {};
	}

	std::string get_vehicle_display_name(const VehicleItem*)
	{
		return {};
	}

	std::string get_vehicle_display_name(const CVehicleModelInfo*)
	{
		return {};
	}
}
