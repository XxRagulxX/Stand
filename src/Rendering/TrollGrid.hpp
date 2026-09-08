#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Players > Troll: mirrors Troll.cpp's general/
	// bounty/vehicle groups exactly - bountyamount now has a real widget
	// (GridItemCommandSlider), same as NetworkGrid's own identical row.
	class TrollGrid : public Grid
	{
	public:
		TrollGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
