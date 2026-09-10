#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsPresetFontGrid : public Grid
	{
	public:
		SettingsPresetFontGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
