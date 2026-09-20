#include "Rendering/SettingsGrid.hpp"

#include "Rendering/GridItemFolder.hpp"
#include "Rendering/HotkeysGrid.hpp"
#include "Rendering/LuaScriptsGrid.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/SettingsAppearanceGrid.hpp"
#include "Rendering/SettingsGameGrid.hpp"
#include "Rendering/SettingsInputGrid.hpp"
#include "Rendering/SettingsProfilesGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		HotkeysGrid g_HotkeysContent{};
		SettingsGameGrid g_GameContent{};
		SettingsAppearanceGrid g_AppearanceContent{};
		SettingsInputGrid g_InputContent{};
		SettingsProfilesGrid g_ProfilesContent{};
		LuaScriptsGrid g_LuaScriptsContent{};
	}

	SettingsGrid::SettingsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Appearance", &g_AppearanceContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Input", &g_InputContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Profiles", &g_ProfilesContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Hotkeys", &g_HotkeysContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Game", &g_GameContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Lua Scripts", &g_LuaScriptsContent));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "console"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "openstandfolder"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "clearstandnotifys"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "emptylog"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "featurelist"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "unload"_J));
	}
}
