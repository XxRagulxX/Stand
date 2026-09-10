#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsInputMouseGrid : public Grid
	{
	public:
		SettingsInputMouseGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
