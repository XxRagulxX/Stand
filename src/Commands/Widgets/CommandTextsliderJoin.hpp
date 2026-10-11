#pragma once

#include "Commands/Widgets/CommandTextslider.hpp"

#include "Commands/Widgets/JoinMethod.hpp"

namespace Stand
{
	class CommandTextsliderJoin : public CommandTextslider
	{
	public:
		explicit CommandTextsliderJoin(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, commandflags_t extra_flags = 0)
			: CommandTextslider(parent, std::move(menu_name), std::move(command_names), NOLABEL, std::vector<CommandTextsliderData>{
				{ JM_NINJA, LOC("JM_N") },
				{ JM_FRIEND, LOC("JM_F") },
			}, CMDFLAGS_ACTION_SLIDER | extra_flags)
		{
		}
	};
}
