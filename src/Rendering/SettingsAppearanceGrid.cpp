#include "Rendering/SettingsAppearanceGrid.hpp"

#include "Commands/Settings/CommandTabAppearanceDirect.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsAddressBarGrid.hpp"
#include "Rendering/SettingsBorderGrid.hpp"
#include "Rendering/SettingsColoursGrid.hpp"
#include "Rendering/SettingsCommandInfoTextGrid.hpp"
#include "Rendering/SettingsCommandsGrid.hpp"
#include "Rendering/SettingsCursorGrid.hpp"
#include "Rendering/SettingsEntityPreviewsGrid.hpp"
#include "Rendering/SettingsFontTextGrid.hpp"
#include "Rendering/SettingsNotificationsGrid.hpp"
#include "Rendering/SettingsPositionGrid.hpp"
#include "Rendering/SettingsScrollbarGrid.hpp"
#include "Rendering/SettingsTabsGrid.hpp"
#include "Rendering/SettingsTexturesGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsAddressBarGrid g_AddressBarContent{};
		SettingsBorderGrid g_BorderContent{};
		SettingsColoursGrid g_ColoursContent{};
		SettingsCommandInfoTextGrid g_CommandInfoTextContent{};
		SettingsCommandsGrid g_CommandsContent{};
		SettingsCursorGrid g_CursorContent{};
		SettingsEntityPreviewsGrid g_EntityPreviewsContent{};
		SettingsFontTextGrid g_FontTextContent{};
		SettingsPositionGrid g_PositionContent{};
		SettingsScrollbarGrid g_ScrollbarContent{};
		SettingsTabsGrid g_TabsContent{};
		SettingsTexturesGrid g_TexturesContent{};
		SettingsNotificationsGrid g_NotificationsContent{};
	}

	SettingsAppearanceGrid::SettingsAppearanceGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsAppearanceGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabAppearanceDirect();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.columns));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.menuheight));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.listwidth));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.listheight));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.spacersize));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.smoothscroll));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.blur));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "openthemefolder"_J));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Colours", &g_ColoursContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Position", &g_PositionContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Address Bar", &g_AddressBarContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Cursor", &g_CursorContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Border", &g_BorderContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Tabs", &g_TabsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Textures", &g_TexturesContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Entity Previews", &g_EntityPreviewsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Font & Text", &g_FontTextContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Command Info Text", &g_CommandInfoTextContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Commands", &g_CommandsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Scrollbar", &g_ScrollbarContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Notifications", &g_NotificationsContent));
	}
}
