#pragma once
#include "Rendering/GridTabbed.hpp"

namespace Stand::Rendering
{
	// Content grid for Vehicle > Spawn: mirrors SpawnVehicle.cpp's
	// BuildSpawnVehicleMenu() TabBarItem in full - New Vehicle/Personal
	// Vehicle tabs (VehicleSpawnNewGrid/VehicleSpawnPersonalGrid), each
	// with its own search-list spawn UI plus Settings toggles.
	class VehicleSpawnGrid : public GridTabbed
	{
	public:
		VehicleSpawnGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
