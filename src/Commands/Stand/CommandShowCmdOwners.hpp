#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandShowCmdOwners : public CommandToggle
	{
	public:
		explicit CommandShowCmdOwners(CommandList* const parent)
			: CommandToggle(parent, LOC("SHWCMDOWNR"), { CMDNAME("showowners") }, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
