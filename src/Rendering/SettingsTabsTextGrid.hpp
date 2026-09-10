#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsTabsTextGrid : public Grid
	{
	public:
		SettingsTabsTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
