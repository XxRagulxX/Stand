#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Settings > Appearance - real Stand's own Appearance category
	// (CommandTabStand.cpp's own settings->createChild<CommandList>
	// (LOC("VIS"))): Colours and Position, each its own folder.
	class SettingsAppearanceGrid : public Grid
	{
	public:
		SettingsAppearanceGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
