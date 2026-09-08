#pragma once
#include "Commands/CommandToggleLegacy.hpp"
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <functional>
#include <memory>

namespace Stand::Rendering
{
	// Wraps another GridItem, gating every one of its calls on a live
	// condition - the Grid equivalent of ConditionalItem (src/
	// ConditionalItem.cpp) for the ImGui menu. Same two ways to supply
	// the condition (a CommandToggle looked up by joaat hash, or an
	// arbitrary std::function<bool()>), same optional negate.
	//
	// Occupies the same x/y/width/height the wrapped item does (copied
	// at construction, then kept in sync via onPositioned() - see
	// below), since this item stands in for it in whatever content
	// Grid's own item list it's pushed into: Grid::setPositions() only
	// ever sees the wrapper, never the item inside it.
	//
	// One real limitation worth being upfront about: when the condition
	// is false, draw()/drawText()/onClick() all no-op (matching
	// ConditionalItem's own CanDraw() gate exactly), but the wrapped
	// item's row still occupies its own slot in the layout - nothing
	// here re-runs Grid::setPositions() to collapse the gap the way
	// Stand's own live update()/updateNow() would. A hidden conditional
	// row is invisible and unclickable, but still leaves a blank strip
	// where it would be. Fixing that for real means the owning content
	// Grid re-populating itself on a condition change, the same way
	// PlayersGrid already does for its own Categories section (see
	// Grid::invalidate()) - not done here, since that's a per-Grid
	// change and this is a single reusable item.
	class GridItemConditional : public GridItem
	{
	public:
		GridItemConditional(std::unique_ptr<GridItem> item, joaat_t boolCommandId, bool negate = false);
		GridItemConditional(std::unique_ptr<GridItem> item, std::function<bool()> conditionFn, bool negate = false);

		void onPositioned() override;

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;
		void onClickEx(int16_t x, int16_t y, bool ctrl, bool shift, bool doubleClick) override;

		bool isFocusable() const override;
		void activate() override;
		bool onArrow(int delta) override;

		bool isEditingText() const override;
		void onChar(wchar_t c) override;
		void onEditKey(unsigned int vkCode) override;

	private:
		[[nodiscard]] bool CanDraw() const;

		CommandToggleLegacy* m_Condition;
		std::function<bool()> m_ConditionFn;
		bool m_Negate;
		std::unique_ptr<GridItem> m_Item;
	};
}
