#pragma once
#include "Rendering/GridItem.hpp"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// A self-contained search box plus scrollable, click-to-select row
	// list over an arbitrary live data source - this project's answer to
	// the classic menu's many ImGui::InputText + ImGui::BeginListBox
	// combinations (Saved Vehicles, Teleport > Saved, World > Spawn
	// Ped's model picker, Weapons > Ammu-Nation, Network > Saved
	// Players, Settings > Lua Scripts, ...). One GridItem covering the
	// whole widget (search field and every row), not a search
	// GridItemTextInput plus N row GridItems - same "populate() only
	// runs once, so anything with a changing row count re-reads its
	// live source on every call instead" reasoning as
	// GridItemPlayerList (see that class's own comment) - rows() is
	// called fresh on every draw()/drawText()/onClickEx()/onArrow(),
	// same as SortedPlayers() there.
	//
	// Filtering is a plain case-insensitive substring match against
	// whatever rows() returns this call, done internally - no separate
	// GridItemTextInput::onChange plumbing needed since there's no
	// external state to keep in sync; the filtered list is just
	// recomputed from the live search buffer on every call instead.
	//
	// The search box shares this item's own isEditingText()/onChar()/
	// onEditKey() (see GridItem.hpp's own doc comments on those) the
	// same way GridItemTextInput does, gated on an internal "currently
	// editing the search text" flag - Enter (activate()) when not
	// already editing opens it for typing, matching GridItemTextInput's
	// own activate(). Unlike GridItemTextInput there's no separate
	// committed value to fall back to: Enter/Escape while editing both
	// just close the box back up, keeping whatever's typed as the live
	// filter.
	//
	// onSelect fires immediately on every click or Left/Right move (not
	// on some separate "confirm" step) - same immediate-effect contract
	// as GridItemPlayerList's onClick/onArrow - carrying whatever
	// gesture produced it so a consumer can tell Vehicle Spawn's plain
	// click-to-spawn apart from World > Spawn Ped's Ctrl+Click-to-set-
	// model or Teleport > Saved's double-click-to-teleport/Shift-click-
	// to-delete without this widget needing to know what any of those
	// mean. A keyboard-driven Left/Right move reads live Left Ctrl state
	// for ctrl (same GetKeyState() pattern MenuGrid's own sidebar
	// navigation uses), so Spawn Ped's Ctrl-gated variant has a keyboard
	// path too; shift/double-click stay false there - no keyboard
	// equivalent for a double-click, and shift would ripple into every
	// other consumer's own shift-gated behaviour as an unrelated side
	// effect. index is into rows()'s *own* result for this call, not the
	// filtered list - what a caller actually needs to act on the right
	// underlying entry.
	//
	// Self-sizing to a fixed maximum number of visible result rows
	// (maxRows) plus the search box itself - with more matches than
	// that, only the first maxRows (in rows()'s own order) show; there's
	// no scrolling within a single GridItem the way Grid itself can
	// scroll between items.
	class GridItemSelectList : public GridItem
	{
	public:
		GridItemSelectList(int16_t width,
		    std::string placeholder,
		    std::function<std::vector<std::string>()> rows,
		    std::function<void(size_t index, const std::string& value, bool ctrl, bool shift, bool doubleClick)> onSelect,
		    int16_t maxRows = 8);

		void draw() override;
		void drawText() override;
		void onClickEx(int16_t cursorX, int16_t cursorY, bool ctrl, bool shift, bool doubleClick) override;

		bool isFocusable() const override
		{
			return true;
		}

		void activate() override;

		// Left(-1)/Right(+1) moves the highlighted row through the
		// filtered list and fires onSelect for it, same immediate-effect
		// contract as GridItemPlayerList's own onArrow - not Up/Down,
		// which already move keyboard focus to a different GridItem
		// entirely. Returns whether it was handled (false with no rows
		// currently matching the filter).
		bool onArrow(int delta) override;

		bool isEditingText() const override
		{
			return m_Editing;
		}

		void onChar(wchar_t c) override;
		void onEditKey(unsigned int vkCode) override;

	private:
		struct FilteredRow
		{
			size_t index; // index into rows()'s own result for this call
			std::string value;
		};
		[[nodiscard]] std::vector<FilteredRow> FilteredRows() const;
		void Select(const FilteredRow& row, bool ctrl, bool shift, bool doubleClick);

		int16_t m_MaxRows;
		std::string m_Placeholder;
		std::function<std::vector<std::string>()> m_Rows;
		std::function<void(size_t, const std::string&, bool, bool, bool)> m_OnSelect;
		std::string m_SearchBuffer;
		bool m_Editing = false;
		size_t m_HighlightedIndex = static_cast<size_t>(-1); // into the filtered list, not rows()
	};
}
