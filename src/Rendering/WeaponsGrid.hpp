#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class WeaponsGrid : public Grid
	{
	public:
		WeaponsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
