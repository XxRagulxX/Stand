#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListRadio : public CommandList
	{
	public:
		explicit CommandListRadio(CommandList* const parent);
	};
}
