#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Recovery > Heists > Apartment Heist: mirrors
	// ApartmentHeist.cpp's RenderApartmentHeistMenu() in full - Heist
	// Cuts, Heist Setups, Hacking and Misc groups, all plain Int/
	// CommandItems (no ConditionalItem gating anywhere in the original).
	//
	// Origin is baked in below the tab strip GridTabbed
	// (RecoveryHeistsGrid) draws above it - see GridTabbed's own class
	// comment for why a tab target hardcodes this itself.
	class ApartmentHeistGrid : public Grid
	{
	public:
		ApartmentHeistGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
