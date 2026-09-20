#include "Rendering/SettingsColoursGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabColours.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsColoursGrid::SettingsColoursGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsColoursGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabColours();

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.primary, "Primary Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rainbow));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.focusText, "Focused Text Colour");
		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.focusRightText, "Focused Right-Bound Text Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyFocusTextToRightText));
		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.focusTexture, "Focused Texture Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyFocusTextToTexture));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.background, "Background Colour");

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.unfocusedText, "Unfocused Text Colour");
		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.unfocusedRightText, "Unfocused Right-Bound Text Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyUnfocusedTextToRightText));
		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.unfocusedTexture, "Unfocused Texture Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyUnfocusedTextToTexture));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.hud, "HUD Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.hudRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimaryToHud));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.ar, "AR Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.arRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimaryToAr));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.minigame, "Minigame Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.minigameRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimaryToMinigame));
	}
}
