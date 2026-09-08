#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Players > Toxic: mirrors Toxic.cpp's damage/
	// griefing groups exactly - every item there is a PlayerCommandItem,
	// now that GridItemCommandPlayer exists.
	class ToxicGrid : public Grid
	{
	public:
		ToxicGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
