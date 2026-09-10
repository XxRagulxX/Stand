#include "Rendering/SettingsGrid.hpp"

#include "Rendering/CustomizeGrid.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/HotkeysGrid.hpp"
#include "Rendering/LuaScriptsGrid.hpp"
#include "Rendering/SettingsAppearanceGrid.hpp"
#include "Rendering/SettingsGameGrid.hpp"
#include "Rendering/SettingsGuiGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid.
		HotkeysGrid g_HotkeysContent{};
		SettingsGuiGrid g_GuiContent{};
		SettingsGameGrid g_GameContent{};
		SettingsAppearanceGrid g_AppearanceContent{};
		CustomizeGrid g_CustomizeContent{};
		LuaScriptsGrid g_LuaScriptsContent{};
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	SettingsGrid::SettingsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Appearance", &g_AppearanceContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Hotkeys", &g_HotkeysContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "GUI", &g_GuiContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Game", &g_GameContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Customize", &g_CustomizeContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Lua Scripts", &g_LuaScriptsContent));
	}
}
