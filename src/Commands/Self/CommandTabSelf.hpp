#pragma once

#include "Commands/Widgets/CommandTab.hpp"

namespace Stand
{
	class CommandTabSelf : public CommandTab
	{
	public:
		explicit CommandTabSelf(CommandList* const parent);

		void populate();
	};
}
