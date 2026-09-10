#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsInputGrid : public Grid
	{
	public:
		SettingsInputGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
