#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Recovery > Heists > Doomsday Heist: mirrors
	// DoomsdayHeist.cpp's RenderDoomsdayHeistMenu() in full - Auto Cuts,
	// Heist Cuts, Heist Setups, Hacking, Misc and Teleport groups, all
	// plain Int/List/CommandItems.
	//
	// Origin is baked in below the tab strip GridTabbed
	// (RecoveryHeistsGrid) draws above it - see GridTabbed's own class
	// comment for why a tab target hardcodes this itself.
	class DoomsdayHeistGrid : public Grid
	{
	public:
		DoomsdayHeistGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
