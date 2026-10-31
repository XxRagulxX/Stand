#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

#include <string>
#include <windows.h>
#include <shellapi.h>

namespace Stand
{
	class CommandReadonlyLink : public CommandPhysical
	{
	public:
		const std::string link;

		explicit CommandReadonlyLink(CommandList* const parent, Label&& menu_name, std::string&& link, Label&& help_text = NOLABEL, std::vector<CommandName>&& command_names = {})
			: CommandPhysical(COMMAND_READONLY_LINK, parent, std::move(menu_name), std::move(command_names), std::move(help_text)),
			  link(std::move(link))
		{
		}

		void onClick(Click& click) override
		{
			ShellExecuteA(nullptr, "open", link.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
		}
	};
}
