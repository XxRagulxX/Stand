#pragma once

#include "Core/AbstractEntity.hpp"
#include "Ped/CustomisationData.hpp"
#include "Vehicle/PositionStateVehData.hpp"
#include "Vehicle/VehicleCustomisationIo.hpp"

namespace Stand
{
	struct TranscendentVehicle
	{
		inline static bool active = false;
		inline static AbstractEntity ent;
		inline static CustomisationData<VehicleCustomisationIo> cust_data;
		inline static v3 pos;
		inline static v3 rot;
		inline static PositionStateVehData extra;

		static void saveFromPlayerState();
		static void save(AbstractEntity& veh);
		static void recover();
		static void removeEntity();
	};
}
