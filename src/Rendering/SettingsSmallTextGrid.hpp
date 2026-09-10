#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsSmallTextGrid : public Grid
	{
	public:
		SettingsSmallTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
