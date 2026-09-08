#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Recovery > Heists > Cayo Perico Heist: mirrors
	// CayoPericoHeist.cpp's RenderCayoPericoHeistMenu() in full - Auto
	// Cuts, Heist Cuts, Heist Setups, Loots, Hacking, Misc and Teleport
	// groups. (Both commented-out ConditionalItem rows in the original
	// stay unported, same as the source.)
	//
	// Origin is baked in below the tab strip GridTabbed
	// (RecoveryHeistsGrid) draws above it - see GridTabbed's own class
	// comment for why a tab target hardcodes this itself.
	class CayoPericoHeistGrid : public Grid
	{
	public:
		CayoPericoHeistGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
