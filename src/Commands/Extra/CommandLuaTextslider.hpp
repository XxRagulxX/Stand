#pragma once

#include "Commands/Extra/CommandLua.hpp"
#include "Commands/Widgets/CommandTextslider.hpp"

namespace Stand
{
	class CommandLuaTextslider : public CommandLuaPhysical<CommandTextslider>
	{
	private:
		const int on_click_ref;

	public:
		explicit CommandLuaTextslider(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, CommandLuaScript* const script, std::vector<CommandTextsliderData>&& options, const int on_click_ref);

		void onClick(Click& click) final;
	};
}
