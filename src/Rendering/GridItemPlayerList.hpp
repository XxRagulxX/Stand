#pragma once
#include "Rendering/GridItem.hpp"

#include <cstdint>

namespace Stand::Rendering
{
	// A live, self-refreshing list of every player in the current
	// session (Players::GetPlayers(), src/Players.hpp), sorted by name -
	// the Grid equivalent of MenuPlayers.cpp's own DrawPlayerList().
	// Clicking a row (or Left/Right while this item has keyboard focus -
	// see onArrow()'s own doc comment for why not Up/Down) selects that
	// player (Players::SetSelected()); the selected row highlights the
	// same way GridItemTabsVertical's active entry does.
	// GridItemCommandPlayer rows elsewhere act on whoever is selected
	// here.
	//
	// One GridItem covering every row, not one row per player the way
	// everything else in this system works (see GridItem.hpp's class
	// comment) - populate() only ever runs once (see Grid.hpp's own
	// class comment on why), but players join and leave throughout a
	// session, so a fixed set of per-player GridItems built at populate
	// time would go stale immediately. This re-reads Players::GetPlayers()
	// itself on every draw()/drawText()/onClick()/onArrow() call instead.
	// Self-sizing to a fixed maximum number of visible rows (kMaxRows) -
	// with more players connected than that, only the first kMaxRows
	// (alphabetically) show; there's no scrolling within a single
	// GridItem the way Grid itself can scroll between items.
	class GridItemPlayerList : public GridItem
	{
	public:
		explicit GridItemPlayerList(int16_t width);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		// Left(-1)/Right(+1) moves the selection up/down the (sorted)
		// player list - not Up/Down, since those already move keyboard
		// focus to a different row/GridItem entirely (see the class
		// comment above for why this can't be one GridItem per player).
		// Returns whether it was handled (false with no players
		// connected at all).
		bool onArrow(int delta) override;

	private:
		static constexpr int16_t kMaxRows = 8;
	};
}
