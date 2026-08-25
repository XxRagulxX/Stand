#pragma once

#include "Commands/Widgets/CommandTab.hpp"

namespace Stand
{
	class CommandTabWorld : public CommandTab
	{
	public:
		explicit CommandTabWorld(CommandList* const parent);

		void populate();
	};
}
