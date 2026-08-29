#pragma once

#include "Commands/Player/CommandOnPlayer.hpp"
#include "Commands/Player/CommandPlayerAction.hpp"

namespace Stand
{
	class CommandPlayerCopyVehicle : public CommandOnPlayer<CommandPlayerAction>
	{
	public:
		explicit CommandPlayerCopyVehicle(CommandList* parent);
		
		void onClick(Click& click) final;
	};
}
