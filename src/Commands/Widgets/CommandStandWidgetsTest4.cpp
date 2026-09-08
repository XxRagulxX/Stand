#include "Commands/Widgets/CommandStandWidgetsTest4.hpp"

#include "Commands/CommandColourCustom.hpp"
#include "Commands/CommandInput.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandTextslider.hpp"

#include <windows.h>

namespace Stand::Features
{
	namespace
	{
		std::pair<int, int> ComputeTestDynamicRange()
		{
			const auto seconds = static_cast<int>(GetTickCount64() / 1000);
			const auto max = 20 + (seconds % 80);
			return {0, max};
		}
	}

	std::pair<int, int> GetTestDynamicRange()
	{
		return ComputeTestDynamicRange();
	}

	static Stand::CommandSliderLegacy _TestDynamicSlider{"standwidgets_testdynamicslider",
	    "Test: Dynamic Range Slider",
	    "Its own max grows/shrinks over time (0 to 20-99) - confirms GetMinimum()/GetMaximum() recompute live via SetDynamicRange() rather than only once at construction",
	    0,
	    100,
	    0};

	static bool _InitTestDynamicSlider = [] {
		_TestDynamicSlider.SetDynamicRange(ComputeTestDynamicRange);
		return true;
	}();

	static Stand::CommandTextslider _TestTextslider{"standwidgets_testtextslider",
	    "Test: Textslider",
	    "A flat left/right value cycler (CommandTextslider) distinct from CommandListSelect - no per-option sub-commands/categories",
	    {"Low", "Medium", "High", "Extreme"},
	    0};

	static Stand::CommandColourCustom _TestColour{"standwidgets_testcolour",
	    "Test: Colour (Hex + HSV)",
	    "Confirms the new Hex row and H/S/V rows stay in sync with the existing R/G/B/A rows",
	    ImVec4(1.0f, 0.0f, 0.0f, 1.0f)};

	static Stand::CommandInput _TestScrollingInput{"standwidgets_testscrollinginput",
	    "Test: Scrolling Text",
	    "Displayed with the marquee-scroll display mode enabled when its text overflows the row",
	    "This text is long enough to overflow the row and should scroll continuously"};
}
