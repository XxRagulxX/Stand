#pragma once
#include "Rendering/StandPort/Grid.hpp"
#include "Rendering/StandPort/Position2d.hpp"

namespace Stand::Rendering
{
	class SettingsCommandsColourCommandsGrid : public Grid
	{
	public:
		SettingsCommandsColourCommandsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
