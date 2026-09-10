#include "Rendering/SettingsColoursGrid.hpp"

#include "Commands/Settings/CommandTabColours.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

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

		AddColorCommandRows(items_draft, Theme::kContentWidth, "primary"_J, "Primary Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rainbow));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "focustext"_J, "Focused Text Colour");
		AddColorCommandRows(items_draft, Theme::kContentWidth, "focusrighttext"_J, "Focused Right-Bound Text Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyFocusTextToRightText));
		AddColorCommandRows(items_draft, Theme::kContentWidth, "focustexture"_J, "Focused Texture Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyFocusTextToTexture));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "background"_J, "Background Colour");

		AddColorCommandRows(items_draft, Theme::kContentWidth, "unfocusedtext"_J, "Unfocused Text Colour");
		AddColorCommandRows(items_draft, Theme::kContentWidth, "unfocusedrighttext"_J, "Unfocused Right-Bound Text Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyUnfocusedTextToRightText));
		AddColorCommandRows(items_draft, Theme::kContentWidth, "unfocusedtexture"_J, "Unfocused Texture Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyUnfocusedTextToTexture));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "hud"_J, "HUD Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.hudRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimaryToHud));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "ar"_J, "AR Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.arRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimaryToAr));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "minigame"_J, "Minigame Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.minigameRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimaryToMinigame));
	}
}
