#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandSavedata : public CommandList
	{
	public:
		explicit CommandSavedata(CommandList* const parent);
	};
}
