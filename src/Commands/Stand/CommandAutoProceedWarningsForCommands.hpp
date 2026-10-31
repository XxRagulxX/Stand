#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandAutoProceedWarningsForCommands : public CommandToggle
	{
	public:
		explicit CommandAutoProceedWarningsForCommands(CommandList* parent)
			: CommandToggle(parent, LOC("B_CB"), { CMDNAME("commandsskipwarnings"), CMDNAME("commandskipswarnings"), CMDNAME("commandskipwarnings") }, LOC("SUPPGEN_CB_H"))
		{
		}

		void onEnable(Click& click) final
		{
		}

		void onDisable(Click& click) final
		{
		}
	};
}
