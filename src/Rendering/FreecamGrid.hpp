#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main > Freecam: the freecam toggle itself
	// plus its one dependent option (freecamspeed, gated on freecam
	// being on) - see InvisibilityGrid.hpp's own comment for why this
	// moved out of SelfGrid's flat list into its own folder page.
	class FreecamGrid : public Grid
	{
	public:
		FreecamGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
