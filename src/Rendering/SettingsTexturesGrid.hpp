#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsTexturesGrid : public Grid
	{
	public:
		SettingsTexturesGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
