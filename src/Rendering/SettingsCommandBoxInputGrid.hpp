#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsCommandBoxInputGrid : public Grid
	{
	public:
		SettingsCommandBoxInputGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
