#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListSpoofing : public CommandList
	{
	public:
		explicit CommandListSpoofing(CommandList* const parent);
	};
}
