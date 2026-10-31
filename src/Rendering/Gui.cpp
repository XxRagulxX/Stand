#include "Rendering/Gui.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Commands/Stand/Game/CommandTabESP.hpp"
#include "Commands/Self/CommandTabSelf.hpp"
#include "Commands/Self/Weapons/CommandTabWeapons.hpp"
#include "Commands/Self/Movement/CommandTabLevitation.hpp"
#include "Commands/Self/Movement/CommandTabMovement.hpp"
#include "Commands/Self/Movement/CommandTabSuperFlight.hpp"
#include "Commands/Self/Movement/CommandTabFloppy.hpp"
#include "Commands/Self/Appearance/CommandTabTransform.hpp"
#include "Commands/Stand/CommandTabStand.hpp"
#include "Commands/Stand/Input/CommandTabInputPresets.hpp"

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

    static void forEachTab(std::function<void(CommandList&)> fn)
    {
        fn(Features::GetCommandTabESP());
        fn(Features::GetCommandTabSelf());
        fn(Features::GetCommandTabWeapons());
        fn(Features::GetCommandTabLevitation());
        fn(Features::GetCommandTabMovement());
        fn(Features::GetCommandTabSuperFlight());
        fn(Features::GetCommandTabFloppy());
        fn(Features::GetCommandTabTransform());
        fn(Features::GetCommandTabStand());
        fn(Features::GetCommandTabInputPresets());
    }

    void Gui::saveStateInMemory(std::unordered_map<std::string, std::string>& state)
    {
        state.clear();
        EXCEPTIONAL_LOCK_READ(g_gui.root_mtx)
        forEachTab([&state](CommandList& tab) {
            std::string prefix = tab.getNameForConfig();
            prefix.push_back('>');
            tab.recursivelySaveStateInMemory(state, std::move(prefix));
        });
        EXCEPTIONAL_UNLOCK_READ(g_gui.root_mtx)
    }

    void Gui::applyDefaultState()
    {
        forEachTab([](CommandList& tab) {
            tab.recursivelyApplyDefaultState();
        });
    }

    void Gui::applyStateFromMemory(std::unordered_map<std::string, std::string> state)
    {
        forEachTab([&state](CommandList& tab) {
            std::string prefix = tab.getNameForConfig();
            prefix.push_back('>');
            tab.recursivelyApplyStateFromMemory(state, std::move(prefix));
        });
    }

    void Gui::lerpStart(CommandList* list) noexcept
    {
        if (lerp != 0)
        {
            lerp_start = get_current_time_millis();
            lerp_from_cursor = list->m_cursor - list->m_offset;
            lerp_from_offset = list->m_offset;
        }
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
