#pragma once
#include "Commands/CommandSliderLegacy.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemFocusTracker.hpp"
#include "Util/Joaat.hpp"

#include <optional>
#include <string>

namespace Stand::Rendering
{
	// Ported from real Stand's own CommandSliderProximity (Commands/
	// Widgets/CommandSliderProximity.hpp on origin/stand-reference) - a
	// radius slider that, only while its own row is focused, draws a
	// translucent debug sphere of that radius at the player's own
	// position every frame - so you can see the range while tuning it,
	// instead of having to guess or leave the menu to check.
	//
	// A subclass of GridItemCommandSlider rather than a stand-alone
	// widget: everything about being a slider row (label, value box,
	// "<"/">" steppers, the Stand-style command box on Enter) is exactly
	// GridItemCommandSlider's own behaviour already - this only adds the
	// sphere, via GridItemFocusTracker (see that class's own comment for
	// why this project's focus-tick hook lives on the GridItem side
	// rather than the Command side real Stand's own version uses).
	// Resolves its own CommandSlider* independently by the same joaat id
	// rather than reading the base class's own private copy (there's no
	// protected accessor for it - GridItemCommandSlider was never
	// designed to be subclassed before this) - a second, harmless lookup
	// of the same command, same trade-off GridItemCommandInput.cpp's own
	// ResolveLabel()/ResolveInitialValue() already accept.
	class GridItemCommandSliderProximity : public GridItemCommandSlider
	{
	public:
		GridItemCommandSliderProximity(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt, int step = 1);

		void draw() override;

	private:
		joaat_t m_Id;
		CommandSliderLegacy* m_Command;
		GridItemFocusTracker m_FocusTracker;
	};
}
