#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandHelpText : public CommandToggle
	{
	public:
		explicit CommandHelpText(CommandList* const parent)
			: CommandToggle(parent, LOC("SHWHLPTXT"), CMDNAMES("showhelptext"), LOC("IPSUM"), true)
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
