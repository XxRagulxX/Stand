#include "Rendering/SettingsAppearanceGrid.hpp"

#include "Rendering/GridItemFolder.hpp"
#include "Rendering/SettingsAddressBarGrid.hpp"
#include "Rendering/SettingsBorderGrid.hpp"
#include "Rendering/SettingsColoursGrid.hpp"
#include "Rendering/SettingsCommandInfoTextGrid.hpp"
#include "Rendering/SettingsCommandsGrid.hpp"
#include "Rendering/SettingsCursorGrid.hpp"
#include "Rendering/SettingsNotificationsGrid.hpp"
#include "Rendering/SettingsPositionGrid.hpp"
#include "Rendering/SettingsScrollbarGrid.hpp"
#include "Rendering/SettingsTabsGrid.hpp"
#include "Rendering/Theme.hpp"

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
		SettingsPositionGrid g_PositionContent{};
		SettingsScrollbarGrid g_ScrollbarContent{};
		SettingsTabsGrid g_TabsContent{};
		SettingsNotificationsGrid g_NotificationsContent{};
	}

	SettingsAppearanceGrid::SettingsAppearanceGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsAppearanceGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Colours", &g_ColoursContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Position", &g_PositionContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Address Bar", &g_AddressBarContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Cursor", &g_CursorContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Border", &g_BorderContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Tabs", &g_TabsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Command Info Text", &g_CommandInfoTextContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Commands", &g_CommandsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Scrollbar", &g_ScrollbarContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Notifications", &g_NotificationsContent));
	}
}
