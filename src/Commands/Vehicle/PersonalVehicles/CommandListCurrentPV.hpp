#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListCurrentPV : public CommandList
	{
	public:
		explicit CommandListCurrentPV(CommandList* parent);
	};
}
