#pragma once
#include "Rendering/StandPort/Grid.hpp"

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
