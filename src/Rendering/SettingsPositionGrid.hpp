#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Settings > Appearance > Position - real Stand's own CommandMenuPosition
	// (X/Y + Move With Mouse) - see Commands/Settings/CommandMenuPosition.cpp.
	class SettingsPositionGrid : public Grid
	{
	public:
		SettingsPositionGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
