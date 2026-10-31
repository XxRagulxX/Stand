#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandSuppressGenericResponsesCommandBox : public CommandToggle
	{
	public:
		explicit CommandSuppressGenericResponsesCommandBox(CommandList* const parent)
			: CommandToggle(parent, LOC("B_CB"), CMDNAMES("suppressgenericresponses"), LOC("SUPPGEN_CB_H"))
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
