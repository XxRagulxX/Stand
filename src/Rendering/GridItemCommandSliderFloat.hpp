#pragma once
#include "Commands/CommandSliderFloatLegacy.hpp"
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <optional>
#include <string>

namespace Stand::Rendering
{
	// A label + current float value (shown to 2 decimal places) + "-"/
	// "+" buttons - the Grid equivalent of FloatCommandItem (src/
	// FloatCommandItem.cpp) for the ImGui menu. See the identical class
	// comment on GridItemCommandSlider for every trade-off here (no text
	// entry, no slider bar - stepping by `step` is the whole
	// interaction) and why it's laid out the same way (right-anchored
	// value + buttons).
	class GridItemCommandSliderFloat : public GridItem
	{
	public:
		GridItemCommandSliderFloat(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt, float step = 0.1f);

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
		// and current value, clamped to [GetMinimum(), GetMaximum()] on
		// submit the same way Step() already is - shared by activate()
		// (Enter) and onClick() (clicking the value box itself, not the
		// "-"/"+" buttons).
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

		CommandSliderFloatLegacy* m_Command;
		std::optional<std::string> m_LabelOverride;
		float m_Step;
	};
}
