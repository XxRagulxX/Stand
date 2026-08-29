#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandListStatEditor : public CommandList
	{
	public:
		explicit CommandListStatEditor(CommandList* const parent);
	};
}