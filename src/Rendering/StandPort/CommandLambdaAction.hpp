#pragma once
#include "Rendering/StandPort/CommandAction.hpp"
#include "Menu/Click.hpp"
#include <functional>

namespace Stand
{
#pragma pack(push, 1)
	class CommandLambdaAction : public CommandAction
	{
	private:
		const std::function<void(Click&)> on_click_impl;

	public:
		explicit CommandLambdaAction(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, std::function<void(Click&)>&& on_click_impl, commandflags_t flags = CMDFLAGS_ACTION, CommandPerm perm = COMMANDPERM_USERONLY)
		    : CommandAction(parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, perm)
		    , on_click_impl(std::move(on_click_impl))
		{
		}

		void onClick(Click& click) final
		{
			on_click_impl(click);
		}
	};
#pragma pack(pop)
}
