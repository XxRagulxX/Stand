#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for the Players sidebar root: mirrors MenuPlayers.cpp's
	// own DrawPlayerList() (now GridItemPlayerList + a GridItemCommandToggle
	// for "spectate"). The category folders (Info/Troll/Toxic/Kick) only
	// show once a player is actually selected - every one of them acts on
	// Players::GetSelected(), so there's nothing for them to do (and, with
	// nobody connected at all, nobody to select in the first place) without
	// one - re-populates itself (see Grid::invalidate()) whenever that
	// changes, rather than always showing them regardless. See InfoGrid's
	// own class comment for its live per-player stats readout.
	class PlayersGrid : public Grid
	{
	public:
		PlayersGrid();

		void draw() override;
		void drawText() override;
		GridItem* findItemAt(int16_t cursorX, int16_t cursorY) override;

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		// Re-populates (via Grid::invalidate()) if whether a player is
		// selected changed since the last call - called from draw()/
		// drawText()/findItemAt(), same "cheap to call every frame,
		// idempotent otherwise" pattern as MenuGrid::SyncNavigation().
		void SyncSelection();

		bool m_LastHasSelection = false;
	};
}
