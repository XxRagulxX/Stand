#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListUnlocks : public CommandList
	{
	public:
		explicit CommandListUnlocks(CommandList* const parent);
	};
}
