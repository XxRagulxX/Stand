#include "Rendering/SettingsInputGrid.hpp"

#include "Commands/Settings/CommandTabInput.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsInputContextHotkeysGrid.hpp"
#include "Rendering/SettingsInputControllerSchemeGrid.hpp"
#include "Rendering/SettingsInputKeepCursorGrid.hpp"
#include "Rendering/SettingsInputKeyboardGrid.hpp"
#include "Rendering/SettingsInputMouseGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsInputKeyboardGrid       g_KeyboardContent{};
		SettingsInputContextHotkeysGrid g_ContextHotkeysContent{};
		SettingsInputMouseGrid          g_MouseContent{};
		SettingsInputControllerSchemeGrid g_ControllerSchemeContent{};
		SettingsInputKeepCursorGrid     g_KeepCursorContent{};
	}

	SettingsInputGrid::SettingsInputGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsInputGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabInput();

		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Keyboard Input Scheme", &g_KeyboardContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Context Option Hotkeys", &g_ContextHotkeysContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Mouse Support", &g_MouseContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.controllerSupport));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Controller Input Scheme", &g_ControllerSchemeContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.backReset));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.backClose));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.interruptHoldWrap));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Keep Cursor When Revisiting", &g_KeepCursorContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.keyRepeatInterval));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.keyLongRepeatInterval));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.tabRepeatInterval));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.disableHotkeys));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.modifierKeyMatching));
	}
}
