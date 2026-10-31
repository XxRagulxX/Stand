#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandControllerSupport : public CommandToggle
	{
	public:
		explicit CommandControllerSupport(CommandList* const parent)
			: CommandToggle(parent, LOC("CTRLLRSPPRT"), CMDNAMES("controller"), NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
