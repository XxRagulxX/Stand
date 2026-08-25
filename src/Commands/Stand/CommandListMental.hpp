#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListMental : public CommandList
	{
	public:
		CommandSlider* mental;
		CommandToggleNoCorrelation* pulse;

		explicit CommandListMental(CommandList* const parent);
	};
}
