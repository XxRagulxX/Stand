#include "Rendering/GridItemList.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/ThemeIcons.hpp"

namespace Stand
{
    void DrawCommandData::setSlider(std::wstring&& value, std::wstring& name, float& command_name_max_width)
    {
        using namespace Rendering;
        if (g_MenuGrid.number_sliders_rightbound_values)
        {
            rightbound_text = std::move(value);
            auto ms = GridRenderer::MeasureText("", Theme::kTextScale);
            rightbound_text_width = (float)rightbound_text.size() * ms.x * 0.6f;
            command_name_max_width -= rightbound_text_width;
        }
        else
        {
            name.append(L": ").append(std::move(value));
        }
    }

    GridItemList::GridItemList(CommandList* view, int16_t height, uint8_t priority,
        Rendering::Alignment alignment_relative_to_last, cursor_t offset)
        : GridItem(Rendering::GRIDITEM_LIST,
            Rendering::Theme::kContentWidth, height, priority, alignment_relative_to_last),
          view(view), offset(offset)
    {
        update();
    }

    bool GridItemList::trimTextH(std::wstring& text, float scale, float maxWidth)
    {
        using namespace Rendering;
        std::string utf8(text.begin(), text.end());
        auto ms = GridRenderer::MeasureText(utf8.c_str(), scale);
        if (ms.x <= maxWidth)
            return false;

        while (!text.empty())
        {
            text.pop_back();
            std::string narrow(text.begin(), text.end());
            narrow += "...";
            auto m2 = GridRenderer::MeasureText(narrow.c_str(), scale);
            if (m2.x <= maxWidth)
            {
                text.assign(narrow.begin(), narrow.end());
                return true;
            }
        }
        return true;
    }

    void GridItemList::update()
    {
        using namespace Rendering;

        m_draw_data = DrawListData{};
        cursor_t emul_cursor = m_draw_data.offset = view->m_offset + this->offset;
        if (g_gui.lerp != 0 && emul_cursor != 0)
        {
            m_draw_data.has_extra_top = true;
            --emul_cursor;
        }
        cursor_t draw_cursor = 0;
        for (; emul_cursor < (cursor_t)view->children.size(); ++emul_cursor)
        {
            const bool focused = (view->m_cursor == emul_cursor);
            auto* const _command = view->children[emul_cursor].get();
            DrawCommandData draw_data{};

            draw_data.textColour      = focused ? Theme::kFocusText      : Theme::kUnfocusedText;
            draw_data.rightTextColour = focused ? Theme::kFocusRightText  : Theme::kUnfocusedRightText;
            draw_data.focused         = focused;
            draw_data.iconColour      = focused ? Theme::kFocusTexture    : Theme::kUnfocusedTexture;

            std::wstring name{};
            float command_name_max_width = (float)width - 5.f - (float)Theme::kContentItemHeight;

            if (auto* const command = _command->getPhysical())
            {
                name = command->menu_name.getLocalisedUtf16();

                if (command->isToggle())
                {
                    auto* const toggle = command->as<CommandToggleNoCorrelation>();
                    const IconSlot toggleIcon = toggle->m_on ? IconSlot::ToggleOn : IconSlot::ToggleOff;

                    if (g_MenuGrid.leftbound_textures_toggles)
                        draw_data.leftIcon = toggleIcon;
                    else
                        draw_data.rightIcon = toggleIcon;
                }
                else if (command->isList())
                {
                    const auto* const list = static_cast<const CommandList*>(command);

                    if (list->type == COMMAND_LIST_SELECT)
                    {
                        auto cur_val = static_cast<const CommandListSelect*>(command)->getCurrentValueMenuName().getLocalisedUtf16();
                        if (focused)
                        {
                            std::wstring compact = name + L": " + cur_val;
                            name.append(L": < ").append(cur_val).append(L" >");
                            std::wstring trimmed = name;
                            if (trimTextH(trimmed, Theme::kTextScale, command_name_max_width))
                            {
                                trimmed = compact;
                                trimTextH(trimmed, Theme::kTextScale, command_name_max_width);
                            }
                            draw_data.trimmed_name = std::move(trimmed);
                        }
                        else
                        {
                            name.append(L": ").append(cur_val);
                        }
                    }
                    else
                    {
                        bool showIcon = true;
                        if (list->indicator_type == LISTINDICATOR_ARROW_IF_CHILDREN
                            && list->countVisibleChildren() == 0)
                        {
                            showIcon = false;
                        }

                        if (showIcon)
                        {
                            IconSlot iconSlot;
                            if (list->type == COMMAND_LIST_SEARCH)
                            {
                                iconSlot = IconSlot::Search;
                            }
                            else if (list->indicator_type == LISTINDICATOR_OFF)
                            {
                                iconSlot = IconSlot::ToggleOffList;
                            }
                            else if (list->indicator_type == LISTINDICATOR_ON)
                            {
                                iconSlot = IconSlot::ToggleOnList;
                            }
                            else
                            {
                                iconSlot = IconSlot::List;
                            }

                            if (g_MenuGrid.leftbound_textures_nontoggles)
                                draw_data.leftIcon = iconSlot;
                            else
                                draw_data.rightIcon = iconSlot;
                        }
                    }
                }
                else if (command->isSlider())
                {
                    auto* const slider = static_cast<const CommandSlider*>(command);
                    std::wstring value = slider->formatNumber(slider->value);
                    if (focused)
                        value.insert(0, L"< ").append(L" >");
                    draw_data.setSlider(std::move(value), name, command_name_max_width);
                }
                else if (command->type == COMMAND_INPUT)
                {
                    if (g_MenuGrid.leftbound_textures_nontoggles)
                        draw_data.leftIcon = IconSlot::Edit;
                    else
                        draw_data.rightIcon = IconSlot::Edit;
                }
                else if (command->type == COMMAND_READONLY_LINK)
                {
                    if (g_MenuGrid.leftbound_textures_nontoggles)
                        draw_data.leftIcon = IconSlot::Link;
                    else
                        draw_data.rightIcon = IconSlot::Link;
                }
                else if (command->type == COMMAND_DIVIDER)
                {
                    draw_data.centered = true;
                }
                else if (command->type == COMMAND_READONLY_VALUE)
                {
                }
                else if (command->type == COMMAND_TEXTSLIDER || command->type == COMMAND_TEXTSLIDER_STATEFUL)
                {
                }
            }
            else
            {
                name = L"[?]";
            }

            if (draw_data.trimmed_name.empty())
            {
                draw_data.trimmed_name = name;
                trimTextH(draw_data.trimmed_name, Theme::kTextScale, command_name_max_width);
            }

            if (focused)
            {
                if (_command->shouldShowUntrimmedName())
                    g_MenuGrid.untrimmed_menu_name = name;
                else
                    g_MenuGrid.untrimmed_menu_name.clear();
            }

            m_draw_data.list.emplace_back(std::move(draw_data));
            if (++draw_cursor == (g_gui.command_rows + (g_gui.lerp != 0) + m_draw_data.has_extra_top))
                break;
        }

        if (m_draw_data.list.empty())
            m_draw_data.has_extra_top = false;
    }

    bool GridItemList::containsCommand(const Command* target) const noexcept
    {
        cursor_t draw_cursor = 0;
        cursor_t emul_cursor = view->m_offset + this->offset;
        while (emul_cursor < (cursor_t)view->children.size())
        {
            if (target == view->children[emul_cursor].get())
                return true;
            if (++draw_cursor == g_gui.command_rows)
                break;
            ++emul_cursor;
        }
        return false;
    }

    void GridItemList::draw()
    {
        update();

        using namespace Rendering;
        using namespace Rendering::Theme;

        const float fx = (float)x;
        const float fy = (float)y;
        const float fw = (float)width;

        const auto& dd = m_draw_data;

        int16_t draw_cursor = dd.has_extra_top ? -1 : 0;
        bool any_focused = false;
        float focus_y = 0.f;

        for (const auto& cmd : dd.list)
        {
            if (cmd.focused)
            {
                focus_y = fy + (float)(kContentItemHeight * draw_cursor);
                GridRenderer::DrawRect(fx, fy, fw, (float)(kContentItemHeight * draw_cursor), kPanelBackground);
                GridRenderer::DrawRect(fx, focus_y, fw, (float)kContentItemHeight, kAccent);
                GridRenderer::DrawRect(fx, focus_y + (float)kContentItemHeight,
                    fw, (float)(kContentItemHeight * (int16_t)(dd.list.size() - (int)(g_gui.lerp != 0) - dd.has_extra_top - draw_cursor) - (int)kContentItemHeight),
                    kPanelBackground);

                if (kCursorBorderWidth > 0)
                {
                    const float bw = (float)kCursorBorderWidth;
                    const float ih = (float)kContentItemHeight;
                    GridRenderer::DrawRect(fx,              focus_y,              fw,  bw,      kCursorBorderColour);
                    GridRenderer::DrawRect(fx,              focus_y + ih - bw,    fw,  bw,      kCursorBorderColour);
                    GridRenderer::DrawRect(fx,              focus_y,              bw,  ih,      kCursorBorderColour);
                    GridRenderer::DrawRect(fx + fw - bw,   focus_y,              bw,  ih,      kCursorBorderColour);
                }

                any_focused = true;
                break;
            }
            ++draw_cursor;
        }

        if (!any_focused)
            GridRenderer::DrawRect(fx, fy, fw, (float)height, kPanelBackground);

        draw_cursor = dd.has_extra_top ? -1 : 0;
        const float icon_size = (float)(kContentItemHeight - 4);
        for (const auto& cmd : dd.list)
        {
            const float item_y = fy + (float)(kContentItemHeight * draw_cursor) + 2.f;

            if (cmd.leftIcon != Rendering::IconSlot::Count)
                Rendering::ThemeIcons::QueueDraw(cmd.leftIcon, fx + 2.f, item_y, icon_size, cmd.iconColour);

            if (cmd.rightIcon != Rendering::IconSlot::Count)
                Rendering::ThemeIcons::QueueDraw(cmd.rightIcon, fx + fw - (float)kContentItemHeight + 2.f, item_y, icon_size, cmd.iconColour);

            ++draw_cursor;
        }
    }

    void GridItemList::drawText()
    {
        using namespace Rendering;
        using namespace Rendering::Theme;

        const float fx = (float)x;
        const float fy = (float)y;
        const float fw = (float)width;
        const auto& dd = m_draw_data;

        int16_t draw_cursor = dd.has_extra_top ? -1 : 0;
        for (const auto& cmd : dd.list)
        {
            float draw_y = fy + (float)(kContentItemHeight * draw_cursor);
            float text_x = fx + (float)kContentItemHeight;
            std::string utf8(cmd.trimmed_name.begin(), cmd.trimmed_name.end());

            if (cmd.centered)
            {
                float tw = GridRenderer::MeasureText(utf8.c_str(), kTextScale).x;
                GridRenderer::DrawText(fx + (fw - tw) * 0.5f, draw_y, utf8.c_str(),
                    cmd.textColour, kTextScale);
            }
            else
            {
                GridRenderer::DrawText(text_x, draw_y, utf8.c_str(), cmd.textColour, kTextScale);
            }

            if (!cmd.rightbound_text.empty())
            {
                std::string rtext(cmd.rightbound_text.begin(), cmd.rightbound_text.end());
                float rx = fx + fw - cmd.rightbound_text_width - 5.f;
                GridRenderer::DrawText(rx, draw_y, rtext.c_str(), cmd.rightTextColour, kTextScale);
            }

            ++draw_cursor;
        }
    }
}
