#pragma once
#include "Rendering/GridItem.hpp"

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// GridItemCommandListSelect's counterpart for an arbitrary local option list
	// instead of a real Stand::CommandListSelect looked up by joaat hash - the
	// same relationship GridItemIntStepper/GridItemFloatStepper already
	// have to GridItemCommandSlider/GridItemCommandSliderFloat. Needed wherever a
	// short fixed set of options isn't a registered Command at all (e.g.
	// Debug > Globals/Locals' Type picker, cycling SavedVariableBase's own
	// seven VariableType values) - a full GridItemSelectList (search box +
	// scrollable rows) is unnecessary weight for a handful of fixed
	// options that only ever need "next"/"previous".
	//
	// Same "<"/">"-cycling, sequential (not right-anchored) layout as
	// GridItemCommandListSelect - see that class's own doc comment for why.
	// options is fixed for this item's lifetime (populate() only runs
	// once - see Grid.hpp's own class comment); onChange fires whenever
	// the index actually changes, same optional-callback contract as
	// GridItemIntStepper's own.
	class GridItemLocalList : public GridItem
	{
	public:
		GridItemLocalList(int16_t width, int16_t height, std::string label, std::vector<std::string> options, int initialIndex, std::function<void(int)> onChange = nullptr);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		// Left(-1)/Right(+1) cycles the value directly - same effect as
		// clicking the "<"/">" buttons. Always returns true (handled)
		// when options is non-empty, same as GridItemCommandListSelect's own.
		bool onArrow(int delta) override;

		int GetIndex() const
		{
			return m_Index;
		}

	private:
		void Cycle(int direction);

		struct Layout
		{
			float valueX;
			float valueWidth;
			float prevX;
			float nextX;
			float buttonSize;
		};
		Layout ComputeLayout() const;
		const std::string& CurrentItemText() const;
		float MaxItemWidth() const; // cached on first call - options doesn't change at runtime

		std::string m_Label;
		std::vector<std::string> m_Options;
		int m_Index;
		std::function<void(int)> m_OnChange;
		mutable std::optional<float> m_MaxItemWidth;
	};
}
