#pragma once
#include "Rendering/StandPort/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsCommandsNumberSlidersGrid : public Grid
	{
	public:
		SettingsCommandsNumberSlidersGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
