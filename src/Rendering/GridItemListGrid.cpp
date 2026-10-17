#include "Rendering/GridItemListGrid.hpp"

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
        if (m_List && m_Item && !m_List->children.empty())
        {
            const cursor_t childCount = static_cast<cursor_t>(m_List->children.size());
            const cursor_t maxRows = Theme::kMenuHeight > 0 ? static_cast<cursor_t>(Theme::kMenuHeight) : 11;
            m_CommandRows = std::min(maxRows, childCount);
            m_Item->height = static_cast<int16_t>(m_CommandRows * Theme::kContentItemHeight);
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
        if (m_Item)
            m_Item->update();
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

        std::string result = physical->help_text.getLocalisedUtf8();

        if (auto syntax = physical->getCommandSyntax(); !syntax.empty())
        {
            if (!result.empty())
                result += '\n';
            result += StringUtils::utf16_to_utf8(syntax);
        }

        if (cmd->isSlider() && Theme::kShowSliderBehaviour)
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
        else if (cmd->isListSelect())
        {
            auto* ls = static_cast<Stand::CommandListSelect*>(cmd->getPhysical());
            FiberPool::queueJob([ls] {
                Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                ls->onRight(click, false);
            });
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
            auto* ls = static_cast<Stand::CommandListSelect*>(cmd->getPhysical());
            FiberPool::queueJob([ls, delta] {
                Click click(CLICK_MENU, TC_SCRIPT_YIELDABLE);
                if (delta > 0)
                    ls->onRight(click, false);
                else
                    ls->onLeft(click, false);
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
