#include "Rendering/StandWidgetsTestGrid4.hpp"

#include "Commands/Widgets/CommandStandWidgetsTest4.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemCommandInput.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandTextslider.hpp"
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

	StandWidgetsTestGrid4::StandWidgetsTestGrid4() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void StandWidgetsTestGrid4::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Dynamic Range Slider", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "standwidgets_testdynamicslider"_J));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Check Current Range", [] {
			const auto [min, max] = Features::GetTestDynamicRange();
			Notifications::Show("Stand Widgets Test 4", std::format("Current dynamic range: {} to {}", min, max), NotificationType::Info);
		}));

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Textslider", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandTextslider>(Theme::kContentWidth, kItemH, "standwidgets_testtextslider"_J));

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Colour (Hex + HSV)", Theme::kText));
		AddColorCommandRows(items_draft, Theme::kContentWidth, "standwidgets_testcolour"_J);

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Scrolling Text Input", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandInput>(Theme::kContentWidth, kItemH, "standwidgets_testscrollinginput"_J, std::nullopt, true));
	}
}
