#pragma once

#include "Commands/Widgets/CommandTab.hpp"

#include "Commands/Widgets/CommandDivider.hpp"

namespace Stand
{
	class CommandTabPlayers : public CommandTab
	{
	public:
		CommandDivider* divider = nullptr;

		explicit CommandTabPlayers(CommandList* const parent);

		void populate();
	};
}
