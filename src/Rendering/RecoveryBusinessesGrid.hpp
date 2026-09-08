#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Recovery > Businesses: mirrors MenuRecovery.cpp's
	// businessSafe, businessOverlay and businessManager groups in full -
	// showwarehouse/showhangar/showbusinesses/shownightclub are all
	// ConditionalItems gated on businessoverlay being on, now that
	// GridItemConditional exists.
	class RecoveryBusinessesGrid : public Grid
	{
	public:
		RecoveryBusinessesGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
