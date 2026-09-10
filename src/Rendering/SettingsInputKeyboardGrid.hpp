#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsInputKeyboardGrid : public Grid
	{
	public:
		SettingsInputKeyboardGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
