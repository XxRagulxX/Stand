#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsCommandInfoTextGrid : public Grid
	{
	public:
		SettingsCommandInfoTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
