#pragma once

#include "Commands/Stand/CommandPlayerScope.hpp"

namespace Stand
{
	class CommandPlayerScopePointer : public CommandPlayerScope
	{
	public:
		playerscope_t* const ptr;

		explicit CommandPlayerScopePointer(CommandList* parent, playerscope_t* const ptr, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, commandflags_t flags = CMDFLAGS_LIST_SELECT, commandflags_t item_flags = 0)
			: CommandPlayerScope(parent, std::move(menu_name), std::move(command_names), std::move(help_text), *ptr, flags, item_flags), ptr(ptr)
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
			*ptr = (playerscope_t)value;
		}
	};
}
