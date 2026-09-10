#include "Rendering/SettingsInputContextHotkeysGrid.hpp"

#include "Commands/Settings/CommandTabInput.hpp"
#include "Config/HotkeySystem.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemHotkeyCapture.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsInputContextHotkeysGrid::SettingsInputContextHotkeysGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsInputContextHotkeysGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabInput();

		auto addBinding = [&](const char* label, CommandLink* link)
		{
			items_draft.push_back(std::make_unique<GridItemHotkeyCapture>(Theme::kContentWidth, kItemH, label, link));
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Clear", [link] {
				link->m_Chain.clear();
			}));
		};

		addBinding("Save State",                    &tab.ctxSaveState);
		addBinding("Load State",                    &tab.ctxLoadState);
		addBinding("Apply Default State",           &tab.ctxApplyDefault);
		addBinding("Apply Default State To Children", &tab.ctxApplyDefaultChildren);
		addBinding("Set To Min Value",              &tab.ctxSetMin);
		addBinding("Set To Max Value",              &tab.ctxSetMax);
	}
}
