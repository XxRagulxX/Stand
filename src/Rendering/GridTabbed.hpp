#pragma once
#include "Rendering/Grid.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemTabsHorizontal;

	// A Grid whose own content includes a horizontal tab strip, with
	// whichever tab's own target Grid is currently active drawn right
	// alongside it - the Grid-level equivalent of TabBarItem/TabItem
	// (src/TabBarItem.cpp/TabItem.cpp) for the ImGui menu.
	//
	// A distinct mechanism from GridItemFolder/MenuNavigation (which
	// replace this system's *entire* content column when a nested
	// category is entered): a tab strip switches only its own page's
	// content, in place, alongside whatever else that page has - what
	// Vehicle > Spawn's New/Personal split and Recovery > Heists' six
	// heist types need, and what a plain content Grid or a
	// GridItemFolder target can't express on its own.
	//
	// Each tab's target Grid needs its own origin baked in at
	// construction time, positioned to sit right where the tab strip's
	// own content area should start (this class does not reposition a
	// target Grid's items - every other content Grid in this system
	// already hardcodes its own origin the same way, so a tabbed page's
	// targets do too, just offset further down/right to clear the tab
	// strip itself). A subclass builds its own populate() as normal (for
	// whatever comes before/after the tab strip) and calls InitTabs()
	// from it to install the strip and remember each tab's target.
	//
	// One known gap: mouse-wheel scrolling (GridRenderer::WndProcImpl's
	// content->ScrollBy() call) always scrolls *this* Grid's own item
	// list (just the tab strip, in practice), never whichever target is
	// currently active - Grid::ScrollBy()/ScrollToShow() aren't virtual,
	// so a target Grid with more content than fits on screen has no way
	// to be reached by the wheel today. Not fixed here; revisit if a
	// tabbed page's own content actually needs to scroll.
	class GridTabbed : public Grid
	{
	public:
		using Grid::Grid;

		void draw() override;
		void drawText() override;
		GridItem* findItemAt(int16_t cursorX, int16_t cursorY) override;

		// Folds the active tab's own target->getFocusableItems() in
		// after this Grid's own (just the tab strip itself) - without
		// this, MenuFocus's Up/Down could reach the tab strip but never
		// anything inside whichever tab is showing, since a target Grid
		// is a separate Grid instance MenuFocus (which only ever sees
		// "the current content Grid" as one object) has no way to reach
		// into on its own.
		[[nodiscard]] std::vector<GridItem*> getFocusableItems() override;

	protected:
		// Appends the tab strip itself to items_draft (same call site
		// convention as any other item push in a populate() override)
		// and remembers targets for draw()/drawText()/findItemAt() to
		// delegate to based on the strip's own live active index. labels
		// and targets must be the same length. height is the strip's own
		// row height (GridItemTabsHorizontal sizes its own width from
		// labels, same as it always has - there's no way to stretch it
		// to a fixed width).
		void InitTabs(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t height, std::vector<std::string> labels, std::vector<Grid*> targets, size_t activeIndex = 0);

	private:
		GridItemTabsHorizontal* m_Tabs = nullptr;
		std::vector<Grid*> m_Targets;
	};
}
