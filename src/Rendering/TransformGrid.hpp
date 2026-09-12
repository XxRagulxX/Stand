#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class TransformGrid : public Grid
	{
	public:
		TransformGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
