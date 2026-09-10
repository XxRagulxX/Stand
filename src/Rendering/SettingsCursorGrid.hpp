#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsCursorGrid : public Grid
	{
	public:
		SettingsCursorGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
