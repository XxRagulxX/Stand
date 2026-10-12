#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandShowNonuseronly : public CommandToggle
	{
	public:
		explicit CommandShowNonuseronly(CommandList* const parent)
			: CommandToggle(parent, LOC("SHWOTHP"), { CMDNAME("shownonuseronly") }, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
