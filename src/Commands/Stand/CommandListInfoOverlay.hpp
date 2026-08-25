#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListInfoOverlay : public CommandList
	{
	public:
		explicit CommandListInfoOverlay(CommandList* const parent);
	};
}
