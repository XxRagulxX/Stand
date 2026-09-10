#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsTabsGrid : public Grid
	{
	public:
		SettingsTabsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
