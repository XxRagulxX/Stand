#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsNotifySampleGrid : public Grid
	{
	public:
		SettingsNotifySampleGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
