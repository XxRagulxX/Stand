#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListDisables : public CommandList
	{
	public:
		explicit CommandListDisables(CommandList* const parent);
	};
}
