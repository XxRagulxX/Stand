#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main > Super Run: the superrun toggle
	// itself plus its two dependent options (moverateoverride,
	// runsprintswimmultiplier - both gated on superrun being on) - see
	// InvisibilityGrid.hpp's own comment for why this moved out of
	// SelfGrid's flat list into its own folder page.
	class SuperRunGrid : public Grid
	{
	public:
		SuperRunGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
