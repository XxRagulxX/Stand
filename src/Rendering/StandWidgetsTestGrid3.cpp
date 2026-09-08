#include "Rendering/StandWidgetsTestGrid3.hpp"

#include "Commands/Widgets/CommandStandWidgetsTest3.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment.
	StandWidgetsTestGrid3::StandWidgetsTestGrid3() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void StandWidgetsTestGrid3::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Toggle Bitflag (same field, different bits)", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "standwidgets_testbitflag0"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "standwidgets_testbitflag3"_J));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Check Flags Value", [] {
			Notifications::Show("Stand Widgets Test 3",
			    std::format("flags = 0x{:X} (bit0 should only flip with the first toggle, bit3 only with the second)", Features::GetTestFlags()),
			    NotificationType::Info);
		}));
	}
}
