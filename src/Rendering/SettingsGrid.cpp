#include "Rendering/SettingsGrid.hpp"

#include "Commands/Settings/CommandTabSettings.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/HotkeysGrid.hpp"
#include "Rendering/LuaScriptsGrid.hpp"
#include "Rendering/SettingsAppearanceGrid.hpp"
#include "Rendering/SettingsGameGrid.hpp"
#include "Rendering/SettingsInputGrid.hpp"
#include "Rendering/SettingsProfilesGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		HotkeysGrid        g_HotkeysContent{};
		SettingsGameGrid   g_GameContent{};
		SettingsAppearanceGrid g_AppearanceContent{};
		SettingsInputGrid  g_InputContent{};
		SettingsProfilesGrid g_ProfilesContent{};
		LuaScriptsGrid     g_LuaScriptsContent{};
	}

	SettingsGrid::SettingsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabSettings();

		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Appearance",  &g_AppearanceContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Input",       &g_InputContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Profiles",    &g_ProfilesContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Hotkeys",     &g_HotkeysContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Game",        &g_GameContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Lua Scripts", &g_LuaScriptsContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.console));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.openStandFolder));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.clearNotifications));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.emptyLog));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.featureList));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.unload));
	}
}
