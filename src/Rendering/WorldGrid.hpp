#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for World > Main: mirrors MenuWorld.cpp's Kill,
	// Delete, Bring, Weather, Time and Other groups in full, now that
	// GridItemConditional/GridItemCommandSlider exist (setweather's own
	// ConditionalItem, and Time's three CommandSlider-bound steppers).
	//
	// Populate() also appends a GridItemFolder row for each of
	// World's other categories (Spawn Ped, IPLs), each pointing at a
	// real content Grid (SpawnPedGrid/WorldIPLsGrid, owned here since
	// nothing else reaches them) - see each one's own header for what it
	// covers.
	class WorldGrid : public Grid
	{
	public:
		WorldGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
