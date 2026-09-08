#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Recovery > Heists > Kortz Center Heist: mirrors
	// KortzCenterHeist.cpp's RenderKortzCenterHeistMenu() in full - one
	// large Heist Setups group (a single ListCommandItem plus a long run
	// of unconditional toggles - the original's own unlabelled "action"
	// Group is declared but never populated or added, so there's nothing
	// to port for it), Hacking and Misc.
	//
	// Origin is baked in below the tab strip GridTabbed
	// (RecoveryHeistsGrid) draws above it - see GridTabbed's own class
	// comment for why a tab target hardcodes this itself.
	class KortzCenterHeistGrid : public Grid
	{
	public:
		KortzCenterHeistGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
