#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Settings > Appearance > Colours - real Stand's own Colours list
	// (CommandTabStand.cpp) - Primary Colour (+ its own nested Rainbow
	// Mode) and Background Colour. See Commands/Settings/
	// CommandPrimaryColour.cpp/CommandBackgroundColour.cpp for what's
	// ported and what's deliberately deferred (Focused/Unfocused Text
	// Colour, Right-Bound Text Colour, Texture Colour, HUD/AR/Minigame
	// Colour - none of those have a corresponding rendering concept in
	// this project yet).
	class SettingsColoursGrid : public Grid
	{
	public:
		SettingsColoursGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
