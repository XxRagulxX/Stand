#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class FloppyGrid : public Grid
	{
	public:
		FloppyGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
