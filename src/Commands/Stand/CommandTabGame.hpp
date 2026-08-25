#pragma once

#include "Commands/Widgets/CommandTab.hpp"

namespace Stand
{
	class CommandTabGame : public CommandTab
	{
	public:
		explicit CommandTabGame(CommandList* const parent);

		void populate();
	};
}
