#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SuperFlightGrid : public Grid
	{
	public:
		SuperFlightGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
