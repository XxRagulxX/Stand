#pragma once

#include "Commands/Widgets/CommandTab.hpp"

namespace Stand
{
	class CommandTabOnline : public CommandTab
	{
	public:
		explicit CommandTabOnline(CommandList* const parent);

		void populate();
	};
}
