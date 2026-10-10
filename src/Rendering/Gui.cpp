#include "Rendering/Gui.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuNavigation.hpp"

namespace Stand
{
    Gui g_gui{};

    CommandList* Gui::getCurrentUiList() const noexcept
    {
        if (m_active_list.empty())
            return root_list.get();
        return m_active_list.back();
    }

    Command* Gui::getCurrentMenuFocus() const noexcept
    {
        auto* content = Rendering::MenuNavigation::Current();
        if (!content) return nullptr;
        auto* item = Rendering::MenuFocus::GetFocusedItem(content);
        if (!item) return nullptr;
        auto* scItem = dynamic_cast<Rendering::GridItemStandCommand*>(item);
        if (!scItem) return nullptr;
        return scItem->GetCommand();
    }

    CommandPhysical* Gui::getCurrentMenuFocusPhysical() const noexcept
    {
        auto* focus = getCurrentMenuFocus();
        if (!focus) return nullptr;
        return focus->getPhysical();
    }

    void Gui::updateFocus(ThreadContext thread_context, Direction momentum)
    {
        while (Rendering::MenuNavigation::Depth() > m_preOpenNavDepth)
            Rendering::MenuNavigation::Pop();
        if (!m_active_list.empty())
            m_active_list.pop_back();
        root_cursor = m_active_list.empty() ? 0 : static_cast<uint8_t>(m_active_list.size() - 1);
    }

    void Gui::updateActiveFocus(ThreadContext thread_context, Direction momentum, Command* prev_focus)
    {
        m_preOpenNavDepth = Rendering::MenuNavigation::Depth();
    }

    void Gui::processToggleCorrelation(ThreadContext thread_context, ToggleCorrelation_t correlation, bool value)
    {
        Click click(CLICK_BULK, thread_context);
        for (const auto& command : commands_with_correlation)
        {
            if (command->correlation.type == correlation)
                command->setStateBasedOnCorrelation(click, value);
        }
    }
}
