#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListTutorial : public CommandList
	{
	public:
		explicit CommandListTutorial(CommandList* const parent);
	};
}
