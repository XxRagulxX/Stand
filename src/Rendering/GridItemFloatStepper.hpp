#pragma once
#include "Rendering/GridItem.hpp"

#include <functional>
#include <string>

namespace Stand::Rendering
{
	// GridItemIntStepper's float counterpart - same label + current value
	// (shown to 2 decimal places, matching GridItemCommandSliderFloat) + "-"/"+"
	// buttons, for a value that isn't backed by a real Command at all
	// (GridItemCommandSliderFloat's own role for a CommandSliderFloat looked up by
	// joaat hash) - the same relationship GridItemIntStepper already has
	// to GridItemCommandSlider. Value is clamped to [min, max], stepping by
	// `step` per click/arrow (0.1f default, same as GridItemCommandSliderFloat's
	// own default).
	//
	// onChange is optional, same contract as GridItemIntStepper's own -
	// a consumer applying the new value via something other than this
	// widget's own displayed state (e.g. Debug > Globals/Locals' live
	// variable editor, writing straight into game memory) supplies one;
	// nothing needing to react instantly just leaves it null.
	class GridItemFloatStepper : public GridItem
	{
	public:
		GridItemFloatStepper(int16_t width, int16_t height, std::string label, float initialValue, float min, float max, float step = 0.1f, std::function<void(float)> onChange = nullptr) :
		    GridItem(GRIDITEM_INDIFFERENT, width, height),
		    m_Label(std::move(label)),
		    m_Value(initialValue),
		    m_Min(min),
		    m_Max(max),
		    m_Step(step),
		    m_OnChange(std::move(onChange))
		{
		}

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		// Left(-1)/Right(+1) steps the value directly, clamped to
		// [min, max] same as the +/- buttons - see the identical doc
		// comment on GridItemIntStepper::onArrow(). Always returns true
		// (handled).
		bool onArrow(int delta) override;

		float GetValue() const
		{
			return m_Value;
		}

	private:
		void Step(int direction);

		struct Layout
		{
			float valueX;
			float valueWidth;
			float minusX;
			float plusX;
			float buttonSize;
		};
		Layout ComputeLayout() const;

		std::string m_Label;
		float m_Value;
		float m_Min;
		float m_Max;
		float m_Step;
		std::function<void(float)> m_OnChange;
	};
}
