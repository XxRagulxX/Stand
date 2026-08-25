#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandTrafficColour : public CommandList
	{
	public:
		explicit CommandTrafficColour(CommandList* const parent);
	};
}
