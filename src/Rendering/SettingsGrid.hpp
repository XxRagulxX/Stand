#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsGrid : public Grid
	{
	public:
		SettingsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
