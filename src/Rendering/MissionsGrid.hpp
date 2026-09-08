#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Recovery > Heists > Missions: mirrors Missions.cpp's
	// RenderMissionsMenu() in full - ten List+Command(s) groups (Low
	// Rider, Casino, Yach, Papertrail, Dragwars, Mercenaries, Cluckin
	// Bell, Tuners, Contract, Savage Yard), all plain List/CommandItems.
	// Every group header here uses the same name as the original Group
	// (including its own "Yach" typo, kept verbatim for parity with the
	// source rather than silently fixed).
	//
	// Origin is baked in below the tab strip GridTabbed
	// (RecoveryHeistsGrid) draws above it - see GridTabbed's own class
	// comment for why a tab target hardcodes this itself.
	class MissionsGrid : public Grid
	{
	public:
		MissionsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
