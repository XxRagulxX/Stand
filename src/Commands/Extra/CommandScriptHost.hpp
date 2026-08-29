#pragma once

#include "Commands/Widgets/CommandAction.hpp"

namespace Stand
{
	class CommandScriptHost : public CommandAction
	{
	public:
		explicit CommandScriptHost(CommandList* const parent);

		void onClick(Click& click) final;
	};
}
