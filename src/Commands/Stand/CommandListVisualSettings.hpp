#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListVisualSettings : public CommandList
	{
	public:
		explicit CommandListVisualSettings(CommandList* const parent);
	};
}
