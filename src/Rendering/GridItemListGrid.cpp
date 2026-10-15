#include "Rendering/GridItemListGrid.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandListRefreshable.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/StandPort/GridItemList.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <windows.h>

namespace Stand::Rendering
{
    GridItemListGrid::GridItemListGrid(Stand::CommandList* list) :
        Grid(Theme::GetContentOrigin(), 0),
        m_List(list)
    {
    }

    GridItemListGrid& GridItemListGrid::GetOrCreate(Stand::CommandList* list)
    {
        static std::unordered_map<Stand::CommandList*, GridItemListGrid> cache;
        return cache.try_emplace(list, list).first->second;
    }

    void GridItemListGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        if (!m_List)
            return;

        int16_t h = static_cast<int16_t>(
            Theme::kHudHeight - static_cast<float>(origin.y) - static_cast<float>(Theme::kContentBottomMargin));
        if (Theme::kListHeight > 0)
            h = std::min(h, Theme::kListHeight);
        if (Theme::kMenuHeight > 0)
            h = std::min(h, static_cast<int16_t>(Theme::kMenuHeight * Theme::kContentItemHeight));
        if (!m_List->children.empty())
            h = std::min(h, static_cast<int16_t>(m_List->children.size() * Theme::kContentItemHeight));

        g_gui.command_rows = static_cast<cursor_t>(h / Theme::kContentItemHeight);
        m_CommandRows = g_gui.command_rows;

        auto item = std::make_unique<GridItemList>(m_List, h, 0, ALIGN_BOTTOM_LEFT, 0);
        m_Item = item.get();
        items_draft.push_back(std::move(item));
    }

    void GridItemListGrid::ScriptTick()
    {
        if (m_CommandRows > 0)
            g_gui.command_rows = m_CommandRows;
        if (m_Item)
            m_Item->update();
        if (m_List && m_Item)
        {
            const int32_t totalH = static_cast<int32_t>(m_List->children.size()) * Theme::kContentItemHeight;
            const int16_t scrollOff = static_cast<int16_t>(m_List->m_offset * Theme::kContentItemHeight);
            setVirtualScroll(totalH, scrollOff);
        }
    }

    void GridItemListGrid::moveCursor(int delta)
    {
        if (!m_List || m_List->children.empty())
            return;

        const cursor_t count = static_cast<cursor_t>(m_List->children.size());
        cursor_t c = m_List->m_cursor + delta;

        if (c < 0)
            c = 0;
        else if (c >= count)
            c = count - 1;

        m_List->m_cursor = c;

        if (c < m_List->m_offset)
            m_List->m_offset = c;
        else if (c >= m_List->m_offset + g_gui.command_rows)
            m_List->m_offset = c - g_gui.command_rows + 1;

    }

    void GridItemListGrid::activateFocused()
    {
        if (!m_List || m_List->children.empty())
            return;

        auto* cmd = m_List->children[m_List->m_cursor].get();
        if (cmd->isList())
        {
            auto* childList = static_cast<Stand::CommandList*>(cmd->getPhysical());
            if (!childList)
                return;
            const std::string label = childList->menu_name.getLocalisedUtf8();
            MenuNavigation::Push(label, &GridItemListGrid::GetOrCreate(childList));
            if (childList->type == COMMAND_LIST_REFRESHABLE)
            {
                FiberPool::queueJob([childList] {
                    auto* refreshable = static_cast<CommandListRefreshable*>(childList);
                    if (refreshable->children.empty())
                        refreshable->populate();
                    refreshable->refresh();
                });
            }
            else
            {
                FiberPool::queueJob([childList] {
                    Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                    childList->onClick(click);
                });
            }
        }
        else if (auto* phys = cmd->getPhysical())
        {
            FiberPool::queueJob([phys] {
                Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                phys->onClick(click);
            });
        }
    }

    void GridItemListGrid::handleMouseHover(int16_t hx, int16_t hy)
    {
        if (!m_List || !m_Item || m_List->children.empty())
            return;
        if (hx < m_Item->x || hx > m_Item->x + m_Item->width)
            return;
        if (hy < m_Item->y || hy > m_Item->y + m_Item->height)
            return;
        const int row = (hy - m_Item->y) / Theme::kContentItemHeight;
        const cursor_t emul = static_cast<cursor_t>(m_List->m_offset + row);
        if (emul >= 0 && emul < static_cast<cursor_t>(m_List->children.size()))
            m_List->m_cursor = emul;
    }

    void GridItemListGrid::handleMouseClick(int16_t hx, int16_t hy, bool /*ctrl*/, bool /*shift*/, bool /*doubleClick*/)
    {
        handleMouseHover(hx, hy);
        activateFocused();
    }

    void GridItemListGrid::handleKey(unsigned int vkCode)
    {
        switch (vkCode)
        {
        case VK_UP:
        case VK_NUMPAD8:
            moveCursor(-1);
            break;
        case VK_DOWN:
        case VK_NUMPAD2:
            moveCursor(1);
            break;
        case VK_RETURN:
        case VK_NUMPAD5:
            activateFocused();
            break;
        default:
            break;
        }
    }
}
