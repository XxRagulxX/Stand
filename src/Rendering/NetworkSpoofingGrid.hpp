#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Network > Spoofing: mirrors MenuNetwork.cpp's
	// matchmakingGroup and matchmakingSrvGroup in full, now that
	// GridItemConditional/GridItemCommandSlider exist - every "enable
	// spoofing this" toggle plus everything it conditionally reveals
	// (mmregion/mmregiontype/mmlanguage/mmplayercount/
	// mmmultiplexsessioncount, and the spoofMMRegion subgroup's own
	// nested condition).
	class NetworkSpoofingGrid : public Grid
	{
	public:
		NetworkSpoofingGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
