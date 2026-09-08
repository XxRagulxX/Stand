#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class StandWidgetsTestGrid4 : public Grid
	{
	public:
		StandWidgetsTestGrid4();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
