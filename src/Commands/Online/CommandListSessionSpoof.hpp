#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListSessionSpoof : public CommandList
	{
	public:
		explicit CommandListSessionSpoof(CommandList* parent);
	};
}
