#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsFontTextGrid : public Grid
	{
	public:
		SettingsFontTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
