#pragma once

#include "Commands/Extra/CommandLua.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	class CommandLuaListSelect : public CommandLuaPhysical<CommandListSelect>
	{
	private:
		const int on_change_ref;

	public:
		explicit CommandLuaListSelect(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, CommandLuaScript* const script, std::vector<CommandListActionItemData>&& options, long long default_value, const int on_change_ref);

		void onChange(Click& click, long long prev_value) final;
	};
}
