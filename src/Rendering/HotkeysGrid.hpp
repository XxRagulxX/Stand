#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Settings > Hotkeys: mirrors MenuSettings.cpp's own
	// Hotkeys()/DrawHotkey.cpp - one row per registered Command that has
	// an entry in HotkeySystem::m_CommandHotkeys (in practice, every
	// registered command - RegisterCommands() seeds one for each up
	// front), sorted by label, same as the original's own sortedCommands.
	//
	// Each command gets a GridItemHotkeyCapture row (see its own class
	// comment for how Enter replaces the original's press-and-hold-the-
	// mouse capture gesture) plus a separate "Clear" button - the
	// original's own per-key click-to-remove chip (a mouse gesture) has
	// no keyboard equivalent added here; Clear + recapturing only the
	// keys still wanted covers the same end result.
	class HotkeysGrid : public Grid
	{
	public:
		HotkeysGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
