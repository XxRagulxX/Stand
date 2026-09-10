#include "Rendering/SettingsInputKeyboardGrid.hpp"

#include "Commands/Settings/CommandTabInput.hpp"
#include "Config/HotkeySystem.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemHotkeyCapture.hpp"
#include "Rendering/SettingsInputPresetsGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsInputPresetsGrid g_PresetsContent{};
	}

	SettingsInputKeyboardGrid::SettingsInputKeyboardGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsInputKeyboardGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabInput();
		auto& scheme = *tab.inputScheme;

		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Presets", &g_PresetsContent));

		auto addBinding = [&](const char* label, CommandLink* link)
		{
			items_draft.push_back(std::make_unique<GridItemHotkeyCapture>(Theme::kContentWidth, kItemH, label, link));
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Clear", [link] {
				link->m_Chain.clear();
			}));
		};

		addBinding("Open/Close Menu", &scheme.keyOpenClose);
		addBinding("Previous Tab",    &scheme.keyPrevTab);
		addBinding("Next Tab",        &scheme.keyNextTab);
		addBinding("Up",              &scheme.keyUp);
		addBinding("Down",            &scheme.keyDown);
		addBinding("Left",            &scheme.keyLeft);
		addBinding("Right",           &scheme.keyRight);
		addBinding("Click",           &scheme.keyClick);
		addBinding("Back",            &scheme.keyBack);
		addBinding("Context Menu",    &scheme.keyContextMenu);
		addBinding("Command Box",     &scheme.keyCommandBox);
	}
}
