#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandForceWarningRead : public CommandToggle
	{
	public:
		explicit CommandForceWarningRead(CommandList* const parent)
			: CommandToggle(parent, LOC("WARNFRCERD"), {}, NOLABEL, true)
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
