#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Stand/PlayerScope.hpp"

namespace Stand
{
	class CommandPlayerScope : public CommandListSelect
	{
	public:
		explicit CommandPlayerScope(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, playerscope_t default_value = PlayerScope::NOONE, commandflags_t flags = CMDFLAGS_LIST_SELECT, commandflags_t item_flags = 0)
			: CommandListSelect(parent, std::move(menu_name), std::move(command_names), std::move(help_text), {}, (long long)default_value, flags)
		{
		}
	};
}
