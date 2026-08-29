#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Commands/Player/CommandPlayerSpectate.hpp"

namespace Stand
{
	class CommandStopSpectating : public CommandAction
	{
	public:
		explicit CommandStopSpectating(CommandList* const parent)
			: CommandAction(parent, LOC("STPSPEC"), CMDNAMES_OBF("stopspectating", "unspectate"))
		{
		}

		void onClick(Click& click) final
		{
			CommandPlayerSpectate::dedup = false;
		}
	};
}
