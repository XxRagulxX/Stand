#include "Rendering/MenuGrid.hpp"

#include "Rendering/GridItemAddressbar.hpp"
#include "Rendering/GridItemTabsHorizontal.hpp"
#include "Rendering/GridItemTabsVertical.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/MiscGrid.hpp"
#include "Rendering/NetworkGrid.hpp"
#include "Rendering/PlayersGrid.hpp"
#include "Rendering/RecoveryGrid.hpp"
#include "Rendering/SelfGrid.hpp"
#include "Rendering/SettingsGrid.hpp"
#include "Rendering/TeleportGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/VehicleGrid.hpp"
#include "Rendering/WorldGrid.hpp"

#include <utility>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		// Stand's own real default_origin (src/Menu/MenuGrid.hpp's
		// MenuGrid::default_origin, { 1323, 560 }), not an arbitrary
		// top-left placement - Stand's menu opens on the right side of
		// the screen, roughly vertically centred, and this was wrongly
		// placed at (20, 20) (top-left) before. Both this and every
		// Theme.hpp geometry constant are H-space coordinates (Stand's
		// own virtual 1920x1080 canvas - see GridRenderer::PosH2C/
		// SizeH2C for how that's mapped onto the real screen
		// resolution), not literal screen pixels.
		//
		// References Theme::kDefaultMenuOriginX/Y rather than restating
		// 1323/560 directly - these stay the fixed base every content
		// Grid's own hardcoded (1438, 587)-style origin assumes, even
		// once Settings > Appearance > Position has moved the menu
		// somewhere else at runtime (Theme::kMenuOriginX/Y) - see
		// Grid::forEachVisibleItem()'s own comment for where that offset
		// actually gets applied.
		constexpr int16_t kHeaderX = Theme::kDefaultMenuOriginX;
		constexpr int16_t kHeaderY = Theme::kDefaultMenuOriginY;

		// Same spacer_size Stand's own MenuGrid uses (Grid(default_origin,
		// 3)) - the gap this Grid's own alignment engine (ported from
		// Stand's Grid::setPositions()) puts between header and sidebar,
		// since both are just items in this Grid's own list now.
		constexpr int16_t kSpacer = Theme::kSpacer;

		// Every content grid's own constructor gets its origin from
		// Theme::GetContentOrigin()/GetTabbedContentOrigin() now - see
		// that function's own doc comment in Theme.hpp for the exact
		// arithmetic (still (1438, 587) for the default Left+visible
		// case). header/sidebar below lay themselves out to match
		// whichever of Theme::kTabsPosition/kTabsVisible produced it.
		std::vector<std::string> SidebarLabels()
		{
			return {
			    "Self",
			    "Vehicle",
			    "Teleport",
			    "Network",
			    "Players",
			    "World",
			    "Recovery",
			    "Settings",
			    "Debug"};
		}

		// Indices into the sidebar's entry list.
		constexpr size_t kSelfIndex = 0;
		constexpr size_t kVehicleIndex = 1;
		constexpr size_t kTeleportIndex = 2;
		constexpr size_t kNetworkIndex = 3;
		constexpr size_t kPlayersIndex = 4;
		constexpr size_t kWorldIndex = 5;
		constexpr size_t kRecoveryIndex = 6;
		constexpr size_t kSettingsIndex = 7;
		constexpr size_t kDebugIndex = 8;

		// The only real content grids this system has so far. Live here
		// (not in GridRenderer.cpp) since MenuGrid is the only thing that
		// decides when any of them is actually shown.
		SelfGrid g_SelfContent{};
		VehicleGrid g_VehicleContent{};
		TeleportGrid g_TeleportContent{};
		NetworkGrid g_NetworkContent{};
		PlayersGrid g_PlayersContent{};
		RecoveryGrid g_RecoveryContent{};
		WorldGrid g_WorldContent{};
		SettingsGrid g_SettingsContent{};
		MiscGrid g_MiscContent{};
	}

	MenuGrid::MenuGrid() :
	    Grid(kHeaderX, kHeaderY, kSpacer)
	{
	}

	MenuGrid::~MenuGrid() = default;

	void MenuGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		const auto position = Theme::kTabsPosition;
		const bool visible = Theme::kTabsVisible;
		const bool sidebarIsVertical = (position == Theme::TabsPosition::Left || position == Theme::TabsPosition::Right);

		// Real Stand's own full_menu_width only grows for LEFT/RIGHT
		// (see origin/stand-reference's src/Menu/MenuGrid.cpp,
		// populate()) - a horizontal (Top/Bottom) sidebar/tab-strip
		// stacks under/over the header instead of beside it, so the
		// header itself only needs to span the content column's own
		// width in that case (or when hidden entirely).
		const int16_t headerW = (visible && sidebarIsVertical) ? static_cast<int16_t>(Theme::kSidebarWidth + kSpacer + Theme::kContentWidth) : Theme::kContentWidth;

		auto header = std::make_unique<GridItemAddressbar>(headerW, Theme::kHeaderHeight, "StandEnhanced");
		m_Header = header.get();
		items_draft.push_back(std::move(header));

		m_Sidebar = nullptr;
		m_SidebarHorizontal = nullptr;

		if (visible)
		{
			// Defaults to Self, the flagship/first page - matches how the
			// real menu opens on Self by default.
			if (sidebarIsVertical)
			{
				auto sidebar = std::make_unique<GridItemTabsVertical>(Theme::kSidebarWidth, Theme::kSidebarEntryHeight, SidebarLabels(), kSelfIndex);

				if (position == Theme::TabsPosition::Right)
				{
					// Mirrored to the right of content instead of stacking
					// under header - not something the alignment engine's
					// own ALIGN_BOTTOM_LEFT/ALIGN_TOP_RIGHT can express on
					// its own here (both keep one axis from the last item;
					// this needs a fixed offset on both), so this item's
					// own x/y are set explicitly and keep_pos tells
					// Grid::setPositions() to leave them alone - same
					// mechanism Stand's own real Grid uses for a
					// caller-positioned item (see its doc comment in
					// Grid.hpp).
					sidebar->x = static_cast<int16_t>(kHeaderX + Theme::kContentWidth + kSpacer);
					sidebar->y = static_cast<int16_t>(kHeaderY + Theme::kHeaderHeight + kSpacer);
					sidebar->keep_pos = true;
				}
				// Left: default ALIGN_BOTTOM_LEFT already stacks this
				// under header, keeping header's own x - unchanged from
				// before this feature existed.

				m_Sidebar = sidebar.get();
				items_draft.push_back(std::move(sidebar));
			}
			else
			{
				auto sidebar = std::make_unique<GridItemTabsHorizontal>(static_cast<float>(Theme::kSidebarEntryHeight), SidebarLabels(), kSelfIndex);

				if (position == Theme::TabsPosition::Bottom)
				{
					// Symmetric to Top (which just stacks under header,
					// same as Left's vertical sidebar does): anchored near
					// the HUD canvas's own bottom edge instead, since
					// content sits directly under header either way (see
					// Theme::GetContentOrigin()) rather than growing to
					// meet wherever this row ends up.
					sidebar->x = kHeaderX;
					sidebar->y = static_cast<int16_t>(Theme::kHudHeight - Theme::kContentBottomMargin - Theme::kSidebarEntryHeight);
					sidebar->keep_pos = true;
				}
				// Top: default ALIGN_BOTTOM_LEFT stacks this under
				// header, same slot Theme::GetContentOrigin()'s own Top
				// case reserves for it before placing content below.

				m_SidebarHorizontal = sidebar.get();
				items_draft.push_back(std::move(sidebar));
			}
		}

		// Every sidebar entry, real content or not - see the class
		// comment in MenuGrid.hpp for why there's no separate "nothing
		// migrated for this one" path any more.
		m_Roots = {
		    {kSelfIndex, "Self", &g_SelfContent},
		    {kVehicleIndex, "Vehicle", &g_VehicleContent},
		    {kTeleportIndex, "Teleport", &g_TeleportContent},
		    {kNetworkIndex, "Network", &g_NetworkContent},
		    {kPlayersIndex, "Players", &g_PlayersContent},
		    {kWorldIndex, "World", &g_WorldContent},
		    {kRecoveryIndex, "Recovery", &g_RecoveryContent},
		    {kSettingsIndex, "Settings", &g_SettingsContent},
		    {kDebugIndex, "Debug", &g_MiscContent},
		};
	}

	bool MenuGrid::GetHeaderBarRect(int16_t& x, int16_t& y, int16_t& width) const
	{
		if (!m_Header)
			return false;

		const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
		const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);

		x = static_cast<int16_t>(m_Header->x + offsetX);
		y = static_cast<int16_t>(m_Header->y + offsetY);
		width = m_Header->width;
		return true;
	}

	bool MenuGrid::GetSidebarRect(int16_t& x, int16_t& y, int16_t& width, int16_t& height) const
	{
		auto* sidebar = SidebarItem();
		if (!sidebar)
			return false;

		const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
		const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);

		x = static_cast<int16_t>(sidebar->x + offsetX);
		y = static_cast<int16_t>(sidebar->y + offsetY);
		width = sidebar->width;
		height = sidebar->height;
		return true;
	}

	GridItem* MenuGrid::SidebarItem() const
	{
		if (m_Sidebar)
			return m_Sidebar;
		if (m_SidebarHorizontal)
			return m_SidebarHorizontal;
		return nullptr;
	}

	size_t MenuGrid::SidebarActiveIndex() const
	{
		if (m_Sidebar)
			return m_Sidebar->GetActiveIndex();
		if (m_SidebarHorizontal)
			return m_SidebarHorizontal->GetActiveIndex();
		return static_cast<size_t>(-1);
	}

	void MenuGrid::MoveSidebarActive(int delta)
	{
		if (m_Sidebar)
			m_Sidebar->MoveActive(delta);
		else if (m_SidebarHorizontal)
			m_SidebarHorizontal->onArrow(delta);
	}

	void MenuGrid::SyncNavigation()
	{
		if (!SidebarItem())
			return;

		const auto activeIndex = SidebarActiveIndex();
		if (activeIndex != m_LastSidebarIndex)
		{
			m_LastSidebarIndex = activeIndex;

			for (auto& root : m_Roots)
			{
				if (root.SidebarIndex == activeIndex)
				{
					MenuNavigation::Reset(root.Label, root.Content);
					break;
				}
			}
		}

		// Unconditional (not just on a sidebar change): a GridItemFolder
		// Push()/Backspace Pop() changes MenuNavigation's own path
		// without touching the sidebar at all, and still needs the
		// breadcrumb to catch up.
		if (m_Header)
			m_Header->SetTitle("StandEnhanced > " + MenuNavigation::BreadcrumbPath());
	}

	void MenuGrid::draw()
	{
		Grid::draw(); // chrome rects: header background + sidebar (also populates on first call)
		SyncNavigation();

		if (auto* content = MenuNavigation::Current())
		{
			// One shared panel background behind the whole content list -
			// matches real Stand's own GridItemList::draw() (confirmed
			// against origin/stand-reference): it paints exactly one
			// bgRectColour rect spanning its own full bounds (split
			// around the focused row's own accent rect, or a single rect
			// covering everything if nothing in it is focused), never a
			// separate fill per row. This project's content Grid had no
			// equivalent - each row only ever draws its own focused-row
			// highlight (see e.g. GridItemFolder::draw()), leaving every
			// unfocused row fully transparent over the game world, unlike
			// the sidebar (GridItemTabsVertical, which does paint a
			// background on every entry regardless of focus - already
			// confirmed correct against Stand's own source, see that
			// class's own comment) - the whole menu reading as one
			// consistent panel means the content side needs this same
			// backdrop, just drawn once instead of per entry. Drawn here,
			// before content->draw(), so every row's own focused-only
			// highlight still layers on top of it exactly like Stand's
			// own focusRectColour rect does over its own bgRectColour.
			const auto visibleHeight = static_cast<int16_t>(Theme::kHudHeight - content->origin.y - Theme::kContentBottomMargin);

			int16_t cx, cy, cw, ch;
			content->getDimensions(cx, cy, cw, ch);
			const auto panelHeight = (ch > 0 && ch < visibleHeight) ? ch : visibleHeight;

			// Same runtime menu-position offset Grid::forEachVisibleItem()
			// applies to every regular item - needed here too since this
			// rect and the scrollbar below are both drawn directly, never
			// going through any Grid's own item list (see that function's
			// own comment in Grid.cpp for why the offset lives there and
			// not in GridRenderer's own PosH2C).
			const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
			const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);

			GridRenderer::DrawRect(static_cast<int16_t>(content->origin.x + offsetX), static_cast<int16_t>(content->origin.y + offsetY), Theme::kContentWidth, panelHeight, Theme::kPanelBackground);

			content->draw();

			m_ContentScrollbar.SetView(content);
			m_ContentScrollbar.x = static_cast<int16_t>(content->origin.x + Theme::kContentWidth + Theme::kScrollbarGap + offsetX);
			m_ContentScrollbar.y = static_cast<int16_t>(content->origin.y + offsetY);
			m_ContentScrollbar.width = Theme::kScrollbarWidth;
			m_ContentScrollbar.height = visibleHeight;
			m_ContentScrollbar.draw();
		}
	}

	void MenuGrid::drawText()
	{
		// Chrome text: header (title already refreshed by this frame's
		// earlier draw() call - see GridRenderer::DrawImpl, which always
		// runs draw() before drawText() within the same frame) + sidebar
		// labels.
		Grid::drawText();

		if (auto* content = MenuNavigation::Current())
			content->drawText();
	}

	GridItem* MenuGrid::findItemAt(int16_t cursorX, int16_t cursorY)
	{
		if (auto* item = Grid::findItemAt(cursorX, cursorY)) // also populates on first call
			return item;

		SyncNavigation();

		if (auto* content = MenuNavigation::Current())
			return content->findItemAt(cursorX, cursorY);

		return nullptr;
	}

	void MenuGrid::HandleKey(unsigned int vkCode)
	{
		// Keeps m_Sidebar/MenuNavigation::Current() in sync before we
		// read either below - same call draw()/findItemAt() already
		// make every frame, safe (and cheap) to call again here too.
		SyncNavigation();

		switch (vkCode)
		{
		case VK_BACK:
		case VK_NUMPAD0:
			// Same "go back" gesture as before this method existed -
			// moved here (from GridRenderer::WndProcImpl directly) so
			// every key this system handles lives in one place. Content
			// focus doesn't need resetting explicitly: MenuFocus detects
			// MenuNavigation::Current() changing underneath it on its
			// own - see MenuFocus.hpp's class comment.
			MenuNavigation::Pop();
			break;

		case VK_UP:
		case VK_NUMPAD8:
		case VK_DOWN:
		case VK_NUMPAD2:
		{
			// Real Stand feel: Up/Down only ever drive the currently-shown
			// submenu's own item list, never the main menu (sidebar) - see
			// VK_CONTROL/VK_SHIFT below for that. Also claims Content focus
			// outright, same as SetFocusedItem() already does for a mouse
			// move/click, so a follow-up Enter activates the item just
			// moved to instead of needing a separate "enter Content" press
			// first.
			if (auto* content = MenuNavigation::Current())
			{
				const int delta = (vkCode == VK_DOWN || vkCode == VK_NUMPAD2) ? 1 : -1;
				MenuFocus::MoveContent(content, delta);
				MenuFocus::SetRegion(MenuFocus::Region::Content);
			}
			break;
		}

		case VK_CONTROL:
		case VK_SHIFT:
			// Real Stand feel: the main menu (sidebar) isn't Up/Down-driven -
			// Right Ctrl moves it down, Right Shift moves it up, from
			// anywhere (not just while focus is already on the sidebar),
			// same as Stand's own scheme. WM_KEYDOWN reports the generic
			// VK_CONTROL/VK_SHIFT for either side, so GetKeyState() (not
			// vkCode) is what tells Right apart from Left here - same
			// pattern GUI::WndProc already uses for its own modifier
			// checks.
			if (SidebarItem())
			{
				if (vkCode == VK_CONTROL && (GetKeyState(VK_RCONTROL) & 0x8000) != 0)
					MoveSidebarActive(1);
				else if (vkCode == VK_SHIFT && (GetKeyState(VK_RSHIFT) & 0x8000) != 0)
					MoveSidebarActive(-1);
			}
			break;

		case VK_LEFT:
		case VK_NUMPAD4:
			// Unhandled (nothing focused, or the focused item has no
			// directly-adjustable value - see GridItem::onArrow()) falls
			// back to moving focus back to the sidebar, the same
			// direction Left points visually (sidebar sits to Content's
			// left).
			if (MenuFocus::GetRegion() == MenuFocus::Region::Content)
			{
				auto* focused = MenuNavigation::Current() ? MenuFocus::GetFocusedItem(MenuNavigation::Current()) : nullptr;
				if (!focused || !focused->onArrow(-1))
					MenuFocus::SetRegion(MenuFocus::Region::Sidebar);
			}
			break;

		case VK_RIGHT:
		case VK_NUMPAD6:
			if (MenuFocus::GetRegion() == MenuFocus::Region::Sidebar)
			{
				MenuFocus::SetRegion(MenuFocus::Region::Content);
			}
			else if (auto* content = MenuNavigation::Current())
			{
				if (auto* focused = MenuFocus::GetFocusedItem(content))
					focused->onArrow(1);
			}
			break;

		case VK_RETURN:
		case VK_NUMPAD5:
			if (MenuFocus::GetRegion() == MenuFocus::Region::Sidebar)
			{
				// Selecting a sidebar entry (Ctrl/Shift - see above) already
				// switches content live, same as Stand's own tab strip;
				// Enter here just moves focus into what's already showing.
				MenuFocus::SetRegion(MenuFocus::Region::Content);
			}
			else if (auto* content = MenuNavigation::Current())
			{
				if (auto* focused = MenuFocus::GetFocusedItem(content))
					focused->activate();
			}
			break;

		default:
			break;
		}

		// Whatever just happened above (a focus move, a page switch, a
		// Backspace pop, ...) may have left keyboard focus on a row
		// that's scrolled out of view - bring it back into view rather
		// than leaving focus on something invisible. Harmless/idempotent
		// otherwise (an already-visible focused item, or no content at
		// all). See Grid::ScrollToShow()'s own doc comment in Grid.hpp.
		if (auto* content = MenuNavigation::Current())
		{
			const auto visibleHeight = static_cast<int16_t>(Theme::kHudHeight - content->origin.y - Theme::kContentBottomMargin);
			content->ScrollToShow(MenuFocus::GetFocusedItem(content), visibleHeight);
		}
	}

	void MenuGrid::HandleMouseMove(int16_t hx, int16_t hy)
	{
		SyncNavigation();

		// Hovering the sidebar just claims Region - unlike a content
		// item, GridItemTabsVertical/GridItemTabsHorizontal is one
		// GridItem for the whole list (see their own class comments), so
		// there's no per-entry focus to set here the way MenuFocus::
		// SetFocusedItem gives content rows.
		if (auto* sidebar = SidebarItem(); sidebar && sidebar->occupies(hx, hy))
		{
			MenuFocus::SetRegion(MenuFocus::Region::Sidebar);
			return;
		}

		if (auto* content = MenuNavigation::Current())
		{
			if (auto* item = content->findItemAt(hx, hy))
				MenuFocus::SetFocusedItem(content, item);
		}
	}

	void MenuGrid::HandleMouseClick(int16_t hx, int16_t hy, bool ctrl, bool shift, bool doubleClick)
	{
		SyncNavigation();

		if (auto* sidebar = SidebarItem(); sidebar && sidebar->occupies(hx, hy))
		{
			MenuFocus::SetRegion(MenuFocus::Region::Sidebar);
			// GridItemTabsVertical/GridItemTabsHorizontal::onClick
			// already works out which entry was hit from the cursor
			// position itself and switches its own active index - no
			// ctrl/shift/doubleClick gesture applies to the sidebar.
			sidebar->onClick(hx, hy);
			return;
		}

		if (auto* content = MenuNavigation::Current())
		{
			if (auto* item = content->findItemAt(hx, hy))
			{
				MenuFocus::SetFocusedItem(content, item);
				item->onClickEx(hx, hy, ctrl, shift, doubleClick);
			}

			// Same "keep focus in view" reasoning as HandleKey()'s own
			// tail - a click can change what content shows (a
			// GridItemFolder push) or scroll position indirectly.
			const auto visibleHeight = static_cast<int16_t>(Theme::kHudHeight - content->origin.y - Theme::kContentBottomMargin);
			content->ScrollToShow(MenuFocus::GetFocusedItem(content), visibleHeight);
		}
	}

	void MenuGrid::HandleMouseWheel(int16_t hx, int16_t hy, int delta)
	{
		// Real Stand feel: the wheel only scrolls the content list, and
		// only while the cursor is actually over it - not the sidebar or
		// header, and not content scrolled out from under a cursor that
		// didn't move.
		auto* content = MenuNavigation::Current();
		if (!content)
			return;

		if (hx < content->origin.x || hx > content->origin.x + Theme::kContentWidth || hy < content->origin.y)
			return;

		const auto visibleHeight = static_cast<int16_t>(Theme::kHudHeight - content->origin.y - Theme::kContentBottomMargin);

		// Wheel-up (positive delta) reveals earlier content, same as
		// every other scrollable view - the opposite sign from
		// ScrollBy's own "positive = later items" convention.
		content->ScrollBy(static_cast<int16_t>(-delta * Theme::kContentItemHeight), visibleHeight);
	}
}
