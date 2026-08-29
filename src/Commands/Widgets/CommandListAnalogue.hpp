#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListAnalogue : public CommandList
	{
	public:
		explicit CommandListAnalogue(CommandList* const parent);
	};
}
