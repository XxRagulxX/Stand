#pragma once

#include "Commands/Widgets/CommandReadonlyLink.hpp"

namespace Stand
{
	class CommandWebOpen : public CommandReadonlyLink
	{
	public:
		explicit CommandWebOpen(CommandList* const parent)
			: CommandReadonlyLink(parent, LOC("WEB"), {}, NOLABEL, { CMDNAME("webinterface") })
		{
		}

		void onClick(Click& click) final
		{
		}
	};
}
