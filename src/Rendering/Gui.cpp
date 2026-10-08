#include "Rendering/Gui.hpp"
#include "Commands/Widgets/CommandIssuable.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "lib/soup/WeakRef.hpp"

namespace Stand
{
    Gui g_gui{};

    CommandList* Gui::getCurrentUiList() const noexcept
    {
        if (m_active_list.empty())
            return root_list.get();
        return m_active_list.back();
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

namespace Stand
{
	bool Gui::parseCommand(std::wstring& command, std::wstring& args)
	{
		return false;
	}

	std::vector<soup::WeakRef<CommandIssuable>> Gui::findCommandsWhereCommandNameStartsWithAsWeakrefs(const CommandName& command_name_prefix, CommandPerm perms) const
	{
		return {};
	}
}
