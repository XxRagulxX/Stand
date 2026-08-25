#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListPatches : public CommandList
	{
	public:
		explicit CommandListPatches(CommandList* const parent);
	};
}
