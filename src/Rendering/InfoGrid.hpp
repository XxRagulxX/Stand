#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Players > Info: mirrors Info.cpp's own two
	// groups - a live readout of whoever's selected (Info.cpp's own
	// "Info" Group) followed by the per-player Teleport group. Only
	// ever shown once a player is actually selected - see PlayersGrid's
	// own class comment - so unlike Info.cpp's ImGuiItem callback, there
	// is no "nobody selected yet, auto-select Self and say so" branch to
	// port: every live row below just falls back to a plain placeholder
	// string if Players::GetSelected() ever goes invalid while this page
	// happens to be open (the player disconnected mid-browse), rather
	// than trying to react to that with a real repopulate.
	class InfoGrid : public Grid
	{
	public:
		InfoGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
