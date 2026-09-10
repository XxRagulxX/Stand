#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsEntityPreviewsGrid : public Grid
	{
	public:
		SettingsEntityPreviewsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
