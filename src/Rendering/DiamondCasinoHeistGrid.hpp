#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Recovery > Heists > Diamond Casino Heist: mirrors
	// DiamondCasinoHeist.cpp's RenderDiamondCasinoHeistMenu() in full -
	// Auto Cuts, Heist Cuts, Heist Setups (including its own nested,
	// unlabelled gunman/driver/weapon/vehicle sub-group, flattened
	// straight into the Setups section here - it was just an unlabelled
	// layout grouping in the original, not a separate page), Loots,
	// Hacking, Misc and Teleport groups. autocollecttargets is the only
	// toggle; everything else is Int/List/CommandItem. (Both commented-
	// out ConditionalItem rows in the original stay unported, same as
	// the source.)
	//
	// Origin is baked in below the tab strip GridTabbed
	// (RecoveryHeistsGrid) draws above it - see GridTabbed's own class
	// comment for why a tab target hardcodes this itself.
	class DiamondCasinoHeistGrid : public Grid
	{
	public:
		DiamondCasinoHeistGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
