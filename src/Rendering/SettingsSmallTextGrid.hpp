#pragma once
#include "Rendering/StandPort/Grid.hpp"
#include "Rendering/StandPort/Position2d.hpp"

namespace Stand::Rendering
{
	class SettingsSmallTextGrid : public Grid
	{
	public:
		SettingsSmallTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
