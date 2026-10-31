#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandChatGadget : public CommandList
	{
	public:
		explicit CommandChatGadget(CommandList* const parent);
	};
}
