#pragma once
#include "Rendering/GridItem.hpp"

#include <string>
#include <vector>

namespace Stand::Rendering
{
	// A row of tab labels with one highlighted as active, mirroring
	// stand-reference's src/Menu/GridItemTabsHorizontal.*. Clicking a tab
	// (or Left/Right while this item has keyboard focus) switches which
	// one is active; GetActiveIndex() lets the owning Grid read the
	// current selection to decide what content to draw - see GridTabbed,
	// the Grid-level container built on top of this for exactly that.
	//
	// Was unused for a while: MenuGrid used this for submenu-category
	// navigation in an earlier version, but Stand's own menu doesn't use
	// a tab strip there at all (see MenuNavigation.hpp/GridItemFolder.hpp
	// for what replaced it there) - kept as a working, self-contained
	// widget since stand-reference itself still carries the type, and
	// now used for real by GridTabbed for the handful of pages that
	// genuinely do have a tab strip in real Stand (Vehicle > Spawn's
	// New/Personal split, Recovery > Heists' six heist types, ...).
	class GridItemTabsHorizontal : public GridItem
	{
	public:
		GridItemTabsHorizontal(float height, std::vector<std::string> tabs, size_t activeIndex) :
		    GridItem(GRIDITEM_TABS, static_cast<int16_t>(GetTotalWidth(tabs)), static_cast<int16_t>(height)),
		    m_Tabs(std::move(tabs)),
		    m_ActiveIndex(activeIndex)
		{
		}

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		// Left(-1)/Right(+1) moves the active tab, clamped (not wrapped)
		// to the tab list's bounds - always returns true (handled) while
		// there's more than one tab.
		bool onArrow(int delta) override;

		size_t GetActiveIndex() const
		{
			return m_ActiveIndex;
		}

	private:
		// Sum of every tab's rendered width + gaps - computed up front
		// (GridItem now needs its width at construction, unlike before
		// this port) rather than lazily after positioning.
		static float GetTotalWidth(const std::vector<std::string>& tabs);

		std::vector<std::string> m_Tabs;
		size_t m_ActiveIndex;
	};
}
