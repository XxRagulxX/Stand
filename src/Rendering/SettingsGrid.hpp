#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Root content grid for the Settings sidebar entry. This root has no
	// direct settings of its own, just a GridItemFolder row for each of
	// Settings' five categories (Hotkeys, GUI, Game, Customize, Lua
	// Scripts), same as every other root's "Categories" section.
	//
	// Every one of Settings' five categories now points at a real
	// content Grid (HotkeysGrid/SettingsGuiGrid/SettingsGameGrid/
	// CustomizeGrid/LuaScriptsGrid, owned here since nothing else
	// reaches them) - see each one's own class comment for what it
	// covers. Hotkeys' and Customize's are both replacements rather
	// than straight ports: Hotkeys for how it replaces the original's
	// single hand-rolled ImGui widget and its mouse-hold capture
	// gesture; Customize for how it replaces the original's ImGuiStyle/
	// ImGuiCol_* editor (meaningless once this Grid renderer is
	// primary) with Theme.hpp's own six colours instead.
	class SettingsGrid : public Grid
	{
	public:
		SettingsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
