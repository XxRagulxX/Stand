#pragma once

#include "Commands/Player/CommandOnPlayer.hpp"
#include "Commands/Widgets/CommandListReadonly.hpp"

namespace Stand
{
	class CommandPlayerInfo : public CommandOnPlayer<CommandListReadonly>
	{
	public:
		explicit CommandPlayerInfo(CommandList* const parent);
	};
}
