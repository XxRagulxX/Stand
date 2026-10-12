#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandShowAddressbar : public CommandToggle
	{
	public:
		explicit CommandShowAddressbar(CommandList* const parent)
			: CommandToggle(parent, LOC("ADDRBAR"), { CMDNAME("addressbar") }, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
