#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main > Noclip: the noclip toggle itself
	// plus its one dependent option (noclipspeed, gated on noclip being
	// on) - see InvisibilityGrid.hpp's own comment for why this moved
	// out of SelfGrid's flat list into its own folder page.
	class NoclipGrid : public Grid
	{
	public:
		NoclipGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
