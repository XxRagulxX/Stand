#pragma once
#include "Commands/Widgets/CommandSliderNamedValue.hpp"
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <optional>
#include <string>

namespace Stand::Rendering
{
	// GridItemCommandSlider's own twin, bound to a StandWidgets::
	// CommandSliderNamedValue instead of a plain CommandSlider - identical in
	// every other respect (same layout, same step/clamp behaviour), but
	// reads the command's own GetDisplayText() for the value box instead
	// of a raw std::to_string(), so the one value the command names
	// (usually its own floor - "Don't Override") shows as that name
	// instead of a number. A separate widget rather than a change to
	// GridItemCommandSlider itself - see CommandSliderNamedValue.hpp's own doc
	// comment for why.
	class GridItemCommandSliderNamedValue : public GridItem
	{
	public:
		GridItemCommandSliderNamedValue(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt, int step = 1);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		bool onArrow(int delta) override;

		// Enter while this item is keyboard-focused - opens the Stand-
		// style command box for typing a value directly instead of only
		// stepping it. See OpenCommandBox()'s own comment.
		void activate() override;

		[[nodiscard]] std::string GetDescription() const override;

	private:
		void Step(int direction);

		// Opens MenuCommandBox prefilled with this command's own name
		// and current raw numeric state (not GetDisplayText() - typing
		// "Don't Override" back in would be a mismatch; the plain number
		// still works to move off it, same as -"/"+" already do),
		// clamped to [GetMinimum(), GetMaximum()] on submit the same way
		// Step() already is - shared by activate() (Enter) and onClick()
		// (clicking the value box itself, not the "-"/"+" buttons).
		void OpenCommandBox();

		struct Layout
		{
			float valueX;
			float valueWidth;
			float minusX;
			float plusX;
			float buttonSize;
		};
		Layout ComputeLayout() const;

		const std::string& Label() const;

		StandWidgets::CommandSliderNamedValue* m_Command;
		std::optional<std::string> m_LabelOverride;
		int m_Step;
	};
}
