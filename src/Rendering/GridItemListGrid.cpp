#include "Rendering/GridItemListGrid.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/GridItemList.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"

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

        const int16_t h = static_cast<int16_t>(
            Theme::kHudHeight - static_cast<float>(origin.y) - static_cast<float>(Theme::kContentBottomMargin));

        g_gui.command_rows = static_cast<cursor_t>(h / Theme::kContentItemHeight);

        auto item = std::make_unique<GridItemList>(m_List, h, 0, ALIGN_BOTTOM_LEFT, 0);
        m_Item = item.get();
        items_draft.push_back(std::move(item));
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
        if (!cmd->isList())
            return;

        auto* childList = static_cast<Stand::CommandList*>(cmd->getPhysical());
        if (!childList)
            return;

        const std::string label = childList->menu_name.getLocalisedUtf8();
        MenuNavigation::Push(label, &GridItemListGrid::GetOrCreate(childList));
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
