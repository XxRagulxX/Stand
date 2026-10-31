#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/GridListSelectOptions.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandListRefreshable.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/StandPort/GridItemList.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Online/CommandPerm.hpp"
#include "Util/StringUtils.hpp"

#include <algorithm>
#include <cmath>
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

        cursor_t children_to_fit = static_cast<cursor_t>(m_List->countVisibleChildren());

        int16_t max_h = static_cast<int16_t>(
            Theme::kHudHeight - static_cast<float>(origin.y) - static_cast<float>(Theme::kContentBottomMargin));
        if (Theme::kListHeight > 0)
            max_h = std::min(max_h, Theme::kListHeight);
        if (Theme::kMenuHeight > 0)
            max_h = std::min(max_h, static_cast<int16_t>(Theme::kMenuHeight * Theme::kContentItemHeight));

        const cursor_t max_rows_from_height = static_cast<cursor_t>(max_h / Theme::kContentItemHeight);

        const cursor_t rows_for_split = (g_gui.command_columns > 1 && children_to_fit > 0)
            ? static_cast<cursor_t>(ceilf(static_cast<float>(children_to_fit) / static_cast<float>(g_gui.command_columns)))
            : max_rows_from_height;
        const cursor_t col_rows = rows_for_split > 0 ? std::min(rows_for_split, max_rows_from_height) : max_rows_from_height;

        g_gui.command_rows = col_rows;
        m_CommandRows = col_rows;

        const int16_t columns_for_this_list = (col_rows > 0 && children_to_fit > 0)
            ? std::clamp<int16_t>(
                static_cast<int16_t>(ceilf(static_cast<float>(children_to_fit) / static_cast<float>(col_rows))),
                1, static_cast<int16_t>(g_gui.command_columns))
            : 1;

        const int16_t first_h = static_cast<int16_t>(Theme::kContentItemHeight * static_cast<int16_t>(std::min(children_to_fit, col_rows)));
        auto item = std::make_unique<GridItemList>(m_List, first_h, 0, ALIGN_BOTTOM_LEFT, 0);
        m_Item = item.get();
        items_draft.push_back(std::move(item));

        for (int16_t i = 1; i < columns_for_this_list; ++i)
        {
            children_to_fit -= col_rows;
            const int16_t col_h = static_cast<int16_t>(Theme::kContentItemHeight * static_cast<int16_t>(std::min(children_to_fit, col_rows)));
            items_draft.push_back(std::make_unique<GridItemList>(m_List, col_h, 0, ALIGN_TOP_RIGHT, col_rows * i));
        }
    }

    void GridItemListGrid::ScriptTick()
    {
        if (m_List && m_Item && !m_List->children.empty())
        {
            const cursor_t childCount = static_cast<cursor_t>(m_List->countVisibleChildren());
            const cursor_t maxRows = Theme::kMenuHeight > 0 ? static_cast<cursor_t>(Theme::kMenuHeight) : 11;
            const cursor_t rows_for_split = (g_gui.command_columns > 1 && childCount > 0)
                ? static_cast<cursor_t>(ceilf(static_cast<float>(childCount) / static_cast<float>(g_gui.command_columns)))
                : maxRows;
            const cursor_t col_rows = rows_for_split > 0 ? std::min(rows_for_split, maxRows) : maxRows;
            m_CommandRows = col_rows;
            const cursor_t col0_children = std::min(childCount, col_rows);
            m_Item->height = static_cast<int16_t>(col0_children * Theme::kContentItemHeight);
        }
        if (m_CommandRows > 0)
            g_gui.command_rows = m_CommandRows;
        if (m_List)
        {
            for (const auto& child : m_List->children)
            {
                if (child->type == Stand::COMMAND_READONLY_VALUE)
                    static_cast<Stand::CommandPhysical*>(child.get())->onTickInGameViewport();
            }
        }
        if (items)
        {
            for (auto& gridItem : *items)
            {
                if (auto* listItem = dynamic_cast<GridItemList*>(gridItem.get()))
                    listItem->update();
            }
        }
        if (m_List && m_Item)
        {
            const int32_t totalH = static_cast<int32_t>(m_List->children.size()) * Theme::kContentItemHeight;
            const int16_t scrollOff = static_cast<int16_t>(m_List->m_offset * Theme::kContentItemHeight);
            setVirtualScroll(totalH, scrollOff);
        }
    }

    std::string GridItemListGrid::GetFocusedDescription() const
    {
        if (!m_List || m_List->children.empty())
            return {};

        auto* cmd = m_List->children[m_List->m_cursor].get();
        if (!cmd)
            return {};

        auto* physical = cmd->getPhysical();
        if (!physical)
            return {};

        std::string result;
        if (g_gui.show_help_text)
            result = physical->help_text.getLocalisedUtf8();

        if (g_gui.show_syntax)
        {
            if (auto syntax = physical->getCommandSyntax(); !syntax.empty())
            {
                if (!result.empty())
                    result += '\n';
                result += StringUtils::utf16_to_utf8(syntax);
            }
        }

        if (cmd->isSlider() && g_gui.show_slider_behaviour)
        {
            auto* slider = static_cast<Stand::CommandSlider*>(cmd->getPhysical());
            if (slider->is_click_to_apply)
            {
                result += '\n';
                result += "Click to apply.";
            }
            else if (!slider->command_names.empty())
            {
                result += '\n';
                result += "Click to input a value.";
            }
            else
            {
                result += '\n';
                result += slider->getRangeString();
            }
        }
        else if (auto* toggle = dynamic_cast<Stand::CommandToggle*>(cmd))
        {
            if (toggle->correlation.isActive())
            {
                auto explanation = toggle->correlation.getExplanation(physical->menu_name.getLocalisedUtf8());
                if (!explanation.empty())
                {
                    if (!result.empty())
                        result += '\n';
                    result += std::move(explanation);
                }
            }
        }
        else if (cmd->isListSelect() && g_gui.show_slider_behaviour)
        {
            auto* sel = static_cast<Stand::CommandListSelect*>(physical);
            if (!result.empty())
                result += '\n';
            result += "Click to select a value.";
            auto valueHelp = sel->getCurrentValueHelpText();
            if (!valueHelp.empty())
            {
                result += '\n';
                result += sel->getCurrentValueMenuName().getLocalisedUtf8();
                result += ": ";
                result += valueHelp.getLocalisedUtf8();
            }
        }

        if (physical->canBeUsedByOtherPlayers() && g_gui.show_nonuseronly)
        {
            auto permLabel = getCommandPermLabel(physical->perm).getLocalisedUtf8();
            if (!permLabel.empty())
            {
                if (!result.empty())
                    result += '\n';
                result += "Other players: ";
                result += std::move(permLabel);
            }
        }

        return result;
    }

    static bool isSectionHeader(Stand::Command* cmd)
    {
        return cmd && (cmd->type == Stand::COMMAND_DIVIDER || (cmd->flags & CMDFLAG_SECTION_HEADER) != 0);
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

        while (c > 0 && c < count - 1 && isSectionHeader(m_List->children[c].get()))
            c += delta > 0 ? 1 : -1;

        if (isSectionHeader(m_List->children[c].get()))
            return;

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
        if (cmd->isListSelect())
        {
            auto* ls = static_cast<Stand::CommandListSelect*>(cmd->getPhysical());
            Rendering::OpenListSelectOptions(ls);
        }
        else if (cmd->isList())
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
        else if (cmd->isSlider())
        {
            auto* slider = static_cast<Stand::CommandSlider*>(cmd->getPhysical());
            if (!slider->command_names.empty())
            {
                const std::string cmdName = slider->command_names[0];
                const std::string menuName = slider->menu_name.getLocalisedUtf8();
                const std::string helpText = slider->help_text.getLocalisedUtf8();
                const std::string description = helpText.empty() ? menuName : menuName + " - " + helpText;
                const std::string initialValue = slider->getValueText();
                MenuCommandBox::Open(
                    cmdName, description, {}, initialValue,
                    [slider](const std::string& val) -> bool {
                        try {
                            int parsed;
                            if (auto* sf = dynamic_cast<Stand::CommandSliderFloat*>(slider))
                            {
                                float fval = std::stof(val);
                                parsed = static_cast<int>(std::roundf(fval * static_cast<float>(sf->getPrecisionScalar())));
                            }
                            else
                            {
                                parsed = std::stoi(val);
                            }
                            FiberPool::queueJob([slider, parsed] {
                                Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                                slider->setValue(click, parsed);
                            });
                            return true;
                        }
                        catch (...) { return false; }
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

    void GridItemListGrid::arrowFocused(int delta)
    {
        if (!m_List || m_List->children.empty())
            return;

        auto* cmd = m_List->children[m_List->m_cursor].get();
        if (cmd->isSlider())
        {
            auto* slider = static_cast<Stand::CommandSlider*>(cmd->getPhysical());
            FiberPool::queueJob([slider, delta] {
                Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                if (delta > 0)
                    slider->onRight(click, false);
                else
                    slider->onLeft(click, false);
            });
        }
        else if (cmd->isListSelect())
        {
            auto* phys = cmd->getPhysical();
            FiberPool::queueJob([phys, delta] {
                Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                if (delta > 0)
                    phys->onRight(click, false);
                else
                    phys->onLeft(click, false);
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
        case VK_LEFT:
        case VK_NUMPAD4:
            arrowFocused(-1);
            break;
        case VK_RIGHT:
        case VK_NUMPAD6:
            arrowFocused(1);
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
