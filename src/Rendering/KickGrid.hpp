#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Players > Kick: mirrors Kick.cpp's kicks/other
	// groups exactly - every item there is a PlayerCommandItem, now that
	// GridItemCommandPlayer exists.
	class KickGrid : public Grid
	{
	public:
		KickGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
