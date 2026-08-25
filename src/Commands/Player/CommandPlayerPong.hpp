#pragma once

#include "Commands/Player/CommandOnPlayer.hpp"
#include "Commands/Widgets/CommandAction.hpp"

namespace Stand
{
	class CommandPlayerPong : public CommandOnPlayer<CommandAction>
	{
	public:
		explicit CommandPlayerPong(CommandList* const parent);

		void onClick(Click& click) final;
	};
}
