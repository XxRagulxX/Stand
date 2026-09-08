#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main > Invisibility: the invis toggle
	// itself plus its one dependent option (localvis, gated on invis
	// being on) - split out of SelfGrid's own flat list into a real
	// folder page since it has an option of its own, same reasoning as
	// every other folder in this file (see SelfGrid.cpp's own comment on
	// why these moved out of GridItemConditional's inline flat-list
	// gating).
	class InvisibilityGrid : public Grid
	{
	public:
		InvisibilityGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
