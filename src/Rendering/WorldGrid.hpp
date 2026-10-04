#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class WorldGrid : public Grid
	{
	public:
		WorldGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
