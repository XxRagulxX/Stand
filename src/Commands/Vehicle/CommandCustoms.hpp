#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandCustoms : public CommandList
	{
	public:
		explicit CommandCustoms(CommandList* const parent);
	};
}
