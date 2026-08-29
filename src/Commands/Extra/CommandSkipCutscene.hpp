#pragma once

#include "Commands/Widgets/CommandAction.hpp"

namespace Stand
{
	class CommandSkipCutscene : public CommandAction
	{
	public:
		explicit CommandSkipCutscene(CommandList* const parent);

		void onClick(Click& click) final;
	};
}
