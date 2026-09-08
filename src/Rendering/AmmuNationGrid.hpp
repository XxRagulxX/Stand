#pragma once
#include "Rendering/GridItem.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace Stand::Rendering
{
	class Grid;

	// Pushes Weapons.cpp's own weaponsAmmuNationGroup rows into
	// items_draft - a free function, not a Grid of its own, since the
	// original has this as just another inline Group inside Self >
	// Weapons' single Main page (between Tools and Gun Van), the same
	// way AddColorCommandRows()/AddVector3CommandRows() push a self-
	// contained cluster of rows for something that isn't a whole
	// separate page either.
	//
	// Mirrors RenderAmmuNationMenu(): a search-list over the same cached
	// weapon name/description/hash list (built once via FiberPool, same
	// pattern as VehicleSpawnNewGrid's own vehicleNames cache), Give
	// Weapon/Remove Weapon buttons, and a live Kills/Deaths/K-D/
	// Headshots/Accuracy readout for whichever weapon is selected
	// (gated on a session being active, same as the original).
	//
	// One simplification, documented at its own call site: the
	// original's own hover tooltip (a weapon's description, shown while
	// hovering its row in the dropdown) becomes a plain live text line
	// showing the *selected* weapon's description instead - there's no
	// hover-tracking primitive in this system yet, and GridItemSelectList
	// only renders plain text rows regardless.
	void AddAmmuNationRows(Grid& grid, std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width);
}
