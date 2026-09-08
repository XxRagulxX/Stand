#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for World > IPLs: mirrors MenuWorld.cpp's iplsGroup -
	// all four items (the iplselector ListCommandItem plus three plain
	// CommandItem buttons) are unconditional, so nothing here is skipped.
	class WorldIPLsGrid : public Grid
	{
	public:
		WorldIPLsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
