#pragma once
#include "Rendering/StandPort/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsCommandsTextGrid : public Grid
	{
	public:
		SettingsCommandsTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
