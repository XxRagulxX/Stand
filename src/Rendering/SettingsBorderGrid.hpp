#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsBorderGrid : public Grid
	{
	public:
		SettingsBorderGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
