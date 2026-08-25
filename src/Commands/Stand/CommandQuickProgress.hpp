#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandQuickProgress : public CommandList
	{
	public:
		explicit CommandQuickProgress(CommandList* const parent);
	};
}
