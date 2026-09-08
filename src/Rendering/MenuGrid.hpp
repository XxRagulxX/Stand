#pragma once
#include "Rendering/Grid.hpp"
#include "Rendering/GridItemScrollbar.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemAddressbar;
	class GridItemTabsVertical;
	class GridItemTabsHorizontal;

	// Top-level chrome: a breadcrumb header + sidebar (submenu list) +
	// whatever content MenuNavigation currently has on top of its stack.
	//
	// Real navigation, replacing the earlier fixed panel where the
	// content area was decorative-only, and - as of this version - also
	// replacing an even earlier horizontal-tab-strip design for
	// switching between a submenu's categories. Stand's own menu has no
	// tab strip: a submenu's "Main" category IS the top-level list, and
	// every other category is just a GridItemFolder row within that same
	// list ("Spawner >", "Garage >", ...) that replaces the content in
	// place when clicked (MenuNavigation::Push()), poppable with
	// Backspace (MenuNavigation::Pop(), wired in GridRenderer.cpp).
	//
	// Every sidebar entry has a root Grid (m_Roots below) - real
	// content in every case now (SelfGrid, VehicleGrid, ...,
	// NetworkGrid, SettingsGrid); there is no placeholder fallback
	// left to fall back to. There's no separate "nothing selected"
	// placeholder path any more: MenuNavigation::Current() is always
	// valid once populate() has run once.
	//
	// header/sidebar are just two GRIDITEM_PRIMARYTEXT/GRIDITEM_TABS
	// items in this Grid's own list now, positioned by the ported
	// alignment engine (sidebar's default ALIGN_BOTTOM_LEFT stacks it
	// under header, keeping header's own x) rather than hardcoded
	// SetPosition() calls - see populate(). Exactly one of m_Sidebar/
	// m_SidebarHorizontal is non-null at a time, depending on
	// Theme::kTabsPosition (GridItemTabsVertical for Left/Right,
	// GridItemTabsHorizontal for Top/Bottom) - both null while
	// Theme::kTabsVisible is false. populate() is rebuilt (via
	// Grid::invalidate()) whenever either changes - see CommandTabs/
	// CommandTabsPos (Commands/Settings/CommandTabs.cpp).
	class MenuGrid : public Grid
	{
	public:
		MenuGrid();
		~MenuGrid() override;

		void draw() override;
		void drawText() override;
		GridItem* findItemAt(int16_t cursorX, int16_t cursorY) override;

		// All keyboard navigation lives here rather than split across
		// GridRenderer/GridItemTabsVertical/MenuFocus - see MenuFocus.hpp
		// for the model (one of Sidebar/Content has focus at a time) and
		// GridItem.hpp for why isFocusable()/activate()/onArrow() exist.
		// vkCode is a plain Win32 virtual-key code (VK_UP, VK_RETURN,
		// ...) - taken as an unsigned rather than WPARAM so this header
		// doesn't need <windows.h>; GridRenderer::WndProcImpl (which
		// already includes it for WM_KEYDOWN itself) casts wparam before
		// calling this.
		void HandleKey(unsigned int vkCode);

		// Real mouse support, same as real Stand has - GridRenderer::
		// WndProcImpl converts the raw WM_MOUSEMOVE/WM_LBUTTONDOWN/
		// WM_MOUSEWHEEL coordinate into H-space (PosC2H) before calling
		// any of these, so hx/hy are already in the same space every
		// GridItem's own x/y/width/height is.
		void HandleMouseMove(int16_t hx, int16_t hy);
		void HandleMouseClick(int16_t hx, int16_t hy, bool ctrl, bool shift, bool doubleClick);
		void HandleMouseWheel(int16_t hx, int16_t hy, int delta);

		// Rebuilds header/sidebar next time populate() runs - called by
		// CommandTabs/CommandTabsPos whenever either changes, the same
		// "call this after changing whatever populate() depends on" idiom
		// Grid::invalidate() already documents for a subclass that needs
		// to decide this for itself.
		void InvalidateLayout()
		{
			invalidate();
		}

		// x/y/width of the breadcrumb header bar, already including the
		// runtime menu-position offset (Theme::kMenuOriginX/Y) - the
		// same offset Grid::forEachVisibleItem() applies to every
		// regular item during draw()/drawText(), but m_Header's own raw
		// x/y is never shifted outside that call (see that function's
		// own comment in Grid.cpp), so GridRenderer (placing the header
		// banner image directly above this bar - same relationship real
		// Stand's own GridItemHeaderAnimation has to its own addressbar,
		// see GridItemHeader.cpp on origin/stand-reference) needs this
		// instead of reading m_Header->x/y directly. Returns false if
		// populate() hasn't run yet (shouldn't happen in practice, but
		// guards against a call before the first draw()).
		bool GetHeaderBarRect(int16_t& x, int16_t& y, int16_t& width) const;

		// x/y/width/height of whichever sidebar (m_Sidebar/
		// m_SidebarHorizontal) is currently active, same runtime-offset
		// handling as GetHeaderBarRect() above - DescriptionPanel's own
		// "below the sidebar" placement needs this (see its own class
		// comment for why there, not beside/below content). Returns
		// false if populate() hasn't run yet or Theme::kTabsVisible is
		// false (no sidebar at all).
		bool GetSidebarRect(int16_t& x, int16_t& y, int16_t& width, int16_t& height) const;

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		// Non-owning - whichever of m_Sidebar/m_SidebarHorizontal is
		// currently non-null, as a plain GridItem* (both provide
		// GetActiveIndex()/occupies()/onClick(), the only things every
		// call site below actually needs regardless of orientation).
		[[nodiscard]] GridItem* SidebarItem() const;
		[[nodiscard]] size_t SidebarActiveIndex() const;
		void MoveSidebarActive(int delta);
		struct SubmenuRoot
		{
			size_t SidebarIndex;
			std::string Label; // breadcrumb root label, e.g. "Vehicle"
			Grid* Content;     // non-owning - see the class comment above
		};

		// Resets MenuNavigation to the sidebar's currently active entry's
		// root, but only when that selection actually changed since the
		// last call - cheap to call every frame (draw()/drawText()/
		// findItemAt() all do), and idempotent otherwise.
		void SyncNavigation();

		GridItemAddressbar* m_Header = nullptr;
		GridItemTabsVertical* m_Sidebar = nullptr;
		GridItemTabsHorizontal* m_SidebarHorizontal = nullptr;
		GridItemScrollbar m_ContentScrollbar;
		std::vector<SubmenuRoot> m_Roots;
		size_t m_LastSidebarIndex = static_cast<size_t>(-1);
	};
}
