#include "Rendering/StandPort/MenuGrid.hpp"

#include "Menu/ContextMenu.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/StandPort/GridItemAddressbar.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/StandPort/GridItemTabsHorizontal.hpp"
#include "Rendering/StandPort/GridItemTabsVertical.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Commands/Self/CommandTabSelf.hpp"
#include "Commands/Vehicle/CommandTabVehicle.hpp"
#include "Commands/World/CommandTabWorld.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

#include "Commands/Widgets/CommandList.hpp"

#include <utility>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		constexpr int16_t kHeaderX = Theme::kDefaultMenuOriginX;
		constexpr int16_t kHeaderY = Theme::kDefaultMenuOriginY;
		constexpr int16_t kSpacer = Theme::kSpacer;

		std::vector<std::string> SidebarLabels()
		{
			return {
			    "Self",
			    "Vehicle",
			    "World",
			    "Stand"};
		}

		constexpr size_t kSelfIndex = 0;
		constexpr size_t kVehicleIndex = 1;
		constexpr size_t kWorldIndex = 2;
		constexpr size_t kStandIndex = 3;

		[[nodiscard]] int16_t ContentVisibleHeight(const Grid* content)
		{
			auto h = static_cast<int16_t>(Theme::kHudHeight - content->origin.y - Theme::kContentBottomMargin);
			if (Theme::kListHeight > 0)
				h = std::min(h, Theme::kListHeight);
			if (Theme::kMenuHeight > 0)
				h = std::min(h, static_cast<int16_t>(Theme::kMenuHeight * Theme::kContentItemHeight));
			return h;
		}

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
		const int16_t headerW = (visible && sidebarIsVertical) ? static_cast<int16_t>(Theme::kTabsWidth + kSpacer + Theme::kContentWidth) : Theme::kContentWidth;

		m_Header = nullptr;
		m_Sidebar = nullptr;
		m_SidebarHorizontal = nullptr;

		if (Theme::kAddressBarVisible)
		{
			auto header = std::make_unique<GridItemAddressbar>(headerW, Theme::kAddressBarHeight, Theme::kAddressBarRootName);
			m_Header = header.get();
			items_draft.push_back(std::move(header));
		}

		if (visible)
		{
			if (sidebarIsVertical)
			{
				const size_t initialTab = (m_LastSidebarIndex != static_cast<size_t>(-1)) ? m_LastSidebarIndex : kSelfIndex;
				auto sidebar = std::make_unique<GridItemTabsVertical>(Theme::kTabsWidth, Theme::kTabsHeight, SidebarLabels(), initialTab);

				if (position == Theme::TabsPosition::Right)
				{
					sidebar->x = static_cast<int16_t>(kHeaderX + Theme::kContentWidth + kSpacer);
					sidebar->y = static_cast<int16_t>(kHeaderY + Theme::kHeaderHeight + kSpacer);
					sidebar->keep_pos = true;
				}

				m_Sidebar = sidebar.get();
				items_draft.push_back(std::move(sidebar));
			}
			else
			{
				const size_t initialTab = (m_LastSidebarIndex != static_cast<size_t>(-1)) ? m_LastSidebarIndex : kSelfIndex;
				auto sidebar = std::make_unique<GridItemTabsHorizontal>(static_cast<float>(Theme::kTabsHeight), SidebarLabels(), initialTab);

				if (position == Theme::TabsPosition::Bottom)
				{
					sidebar->x = kHeaderX;
					sidebar->y = static_cast<int16_t>(Theme::kHudHeight - Theme::kContentBottomMargin - Theme::kTabsHeight);
					sidebar->keep_pos = true;
				}

				m_SidebarHorizontal = sidebar.get();
				items_draft.push_back(std::move(sidebar));
			}
		}

		m_Roots = {
		    {kSelfIndex,     "Self",     &GridItemListGrid::GetOrCreate(&Features::GetCommandTabSelf())},
		    {kVehicleIndex,  "Vehicle",  &GridItemListGrid::GetOrCreate(&Features::GetCommandTabVehicle())},
		    {kWorldIndex,    "World",    &GridItemListGrid::GetOrCreate(&Features::GetCommandTabWorld())},
		    {kStandIndex, "Stand", &GridItemListGrid::GetOrCreate(&Features::GetCommandTabStand())},
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
			// Save focus index and full stack for the departing tab.
			MenuFocus::SaveFor(MenuNavigation::Current());
			if (m_LastSidebarIndex != static_cast<size_t>(-1))
				MenuNavigation::SaveStackFor(m_LastSidebarIndex);

			m_LastSidebarIndex = activeIndex;

			for (auto& root : m_Roots)
			{
				if (root.SidebarIndex == activeIndex)
				{
					if (!MenuNavigation::RestoreStackFor(activeIndex))
						MenuNavigation::Reset(root.Label, root.Content);
					MenuFocus::RestoreFor(MenuNavigation::Current());
					break;
				}
			}
		}

		if (m_Header)
		{
			const auto& sep = Theme::kAddressSeparator;
			const auto& rootName = Theme::kAddressBarRootName;
			const auto path = MenuNavigation::BreadcrumbPath();
			std::string title;
			if (Theme::kAddressCurrentListOnly)
			{
				const auto pos = path.rfind(sep);
				title = pos == std::string::npos ? path : path.substr(pos + sep.size());
				if (title.empty() && !rootName.empty())
					title = rootName;
			}
			else
			{
				if (rootName.empty())
					title = path;
				else
					title = rootName + sep + path;
			}
			m_Header->SetTitle(std::move(title));

			std::string cursorPos;
			if (Theme::kShowCursorPos)
			{
				if (auto* content = MenuNavigation::Current())
				{
					if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
					{
						if (auto* list = listGrid->getCommandList())
						{
							const auto dividers = Theme::kCursorPosIncludesDividers ? 0 : list->dividers;
							const auto total = static_cast<int>(list->children.size()) - dividers;
							const auto cursor = static_cast<int>(list->m_cursor) + 1;
							if (total > 0)
								cursorPos = std::to_string(cursor) + "/" + std::to_string(total);
						}
					}
				}
			}
			m_Header->SetCursorPos(std::move(cursorPos));
		}
	}

	void MenuGrid::draw()
	{
		Grid::draw();
		SyncNavigation();

		if (auto* content = MenuNavigation::Current())
		{
			const auto visibleHeight = ContentVisibleHeight(content);

			int16_t cx, cy, cw, ch;
			content->getDimensions(cx, cy, cw, ch);
			const auto panelHeight = (ch > 0 && ch < visibleHeight) ? ch : visibleHeight;
			const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
			const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);

			GridRenderer::DrawRect(static_cast<int16_t>(content->origin.x + offsetX), static_cast<int16_t>(content->origin.y + offsetY), Theme::kContentWidth, panelHeight, Theme::kPanelBackground);

			content->SetClipMaxY(static_cast<int16_t>(content->origin.y + visibleHeight));
			content->draw();

			m_ContentScrollbar.SetView(content);
			m_ContentScrollbar.x = static_cast<int16_t>(content->origin.x + Theme::kContentWidth + Theme::kScrollbarGap + offsetX);
			m_ContentScrollbar.y = static_cast<int16_t>(content->origin.y + offsetY);
			m_ContentScrollbar.width = Theme::kScrollbarWidthMutable;
			m_ContentScrollbar.height = visibleHeight;
			m_ContentScrollbar.draw();
		}
	}

	void MenuGrid::drawText()
	{
		Grid::drawText();

		if (auto* content = MenuNavigation::Current())
		{
			content->drawText();
		}
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
		SyncNavigation();

		switch (vkCode)
		{
		case VK_BACK:
		case VK_NUMPAD0:
			MenuNavigation::Pop();
			break;

		case VK_UP:
		case VK_NUMPAD8:
		case VK_DOWN:
		case VK_NUMPAD2:
		{
			if (auto* content = MenuNavigation::Current())
			{
				if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
				{
					listGrid->handleKey(vkCode);
				}
				else
				{
					const int delta = (vkCode == VK_DOWN || vkCode == VK_NUMPAD2) ? 1 : -1;
					auto* focused = MenuFocus::GetFocusedItem(content);
					if (!focused || !focused->handleNavigation(delta))
						MenuFocus::MoveContent(content, delta);
					MenuFocus::SetRegion(MenuFocus::Region::Content);
				}
			}
			break;
		}

		case VK_CONTROL:
		case VK_SHIFT:
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
			if (MenuFocus::GetRegion() == MenuFocus::Region::Content)
			{
				if (auto* content = MenuNavigation::Current())
				{
					if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
					{
						listGrid->handleKey(vkCode);
						break;
					}
				}
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
				if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
				{
					listGrid->handleKey(vkCode);
					break;
				}
				if (auto* focused = MenuFocus::GetFocusedItem(content))
					focused->onArrow(1);
			}
			break;

		case VK_RETURN:
		case VK_NUMPAD5:
			if (MenuFocus::GetRegion() == MenuFocus::Region::Sidebar)
			{
				MenuFocus::SetRegion(MenuFocus::Region::Content);
			}
			else if (auto* content = MenuNavigation::Current())
			{
				if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
					listGrid->handleKey(vkCode);
				else if (auto* focused = MenuFocus::GetFocusedItem(content))
					focused->activate();
			}
			break;

		case 0x4F: // O — open command options for the focused item
		{
			if (MenuFocus::GetRegion() == MenuFocus::Region::Content)
			{
				if (auto* content = MenuNavigation::Current())
				{
					if (auto* focused = MenuFocus::GetFocusedItem(content))
					{
						auto* scItem = dynamic_cast<GridItemStandCommand*>(focused);
						if (scItem)
						{
							auto* cmd = scItem->GetCommand();
							if (cmd)
							{
								auto* phys = cmd->getPhysical();
								if (phys)
								{
									ContextMenu::open(TC_OTHER);
									if (ContextMenu::isOpen())
									{
										MenuNavigation::Push(
											ContextMenu::view->menu_name.getLocalisedUtf8(),
											&GridItemListGrid::GetOrCreate(ContextMenu::view));
									}
								}
							}
						}
					}
				}
			}
			break;
		}

		default:
			break;
		}

		if (auto* content = MenuNavigation::Current())
		{
			const auto visibleHeight = ContentVisibleHeight(content);
			content->ScrollToShow(MenuFocus::GetFocusedItem(content), visibleHeight);
		}
	}

	void MenuGrid::HandleMouseMove(int16_t hx, int16_t hy)
	{
		SyncNavigation();

		if (auto* sidebar = SidebarItem(); sidebar && sidebar->occupies(hx, hy))
		{
			MenuFocus::SetRegion(MenuFocus::Region::Sidebar);
			return;
		}

		if (auto* content = MenuNavigation::Current())
		{
			if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
				listGrid->handleMouseHover(hx, hy);
			else if (auto* item = content->findItemAt(hx, hy))
				MenuFocus::SetFocusedItem(content, item);
		}
	}

	void MenuGrid::HandleMouseClick(int16_t hx, int16_t hy, bool ctrl, bool shift, bool doubleClick)
	{
		SyncNavigation();

		if (auto* sidebar = SidebarItem(); sidebar && sidebar->occupies(hx, hy))
		{
			MenuFocus::SetRegion(MenuFocus::Region::Sidebar);
			sidebar->onClick(hx, hy);
			return;
		}

		if (auto* content = MenuNavigation::Current())
		{
			if (auto* listGrid = dynamic_cast<GridItemListGrid*>(content))
			{
				listGrid->handleMouseClick(hx, hy, ctrl, shift, doubleClick);
			}
			else if (auto* item = content->findItemAt(hx, hy))
			{
				MenuFocus::SetFocusedItem(content, item);
				item->onClickEx(hx, hy, ctrl, shift, doubleClick);
			}

			content->ScrollToShow(MenuFocus::GetFocusedItem(content), ContentVisibleHeight(content));
		}
	}

	void MenuGrid::HandleMouseWheel(int16_t hx, int16_t hy, int delta)
	{
		auto* content = MenuNavigation::Current();
		if (!content)
			return;

		if (hx < content->origin.x || hx > content->origin.x + Theme::kContentWidth || hy < content->origin.y)
			return;
		
		content->ScrollBy(static_cast<int16_t>(-delta * Theme::kContentItemHeight), ContentVisibleHeight(content));
	}
}
