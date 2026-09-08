#include "Rendering/SettingsColoursGrid.hpp"

#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment.
	SettingsColoursGrid::SettingsColoursGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsColoursGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Primary Colour + its own nested Rainbow Mode row, same order
		// real Stand's own CommandTabStand.cpp builds them in.
		AddColorCommandRows(items_draft, Theme::kContentWidth, "primary"_J, "Primary Colour");
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "rainbow"_J, "Rainbow Mode"));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "background"_J, "Background Colour");
	}
}
