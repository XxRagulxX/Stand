#pragma once

#include "Commands/Player/CommandPlayerActionAggressive.hpp"

namespace Stand
{
	class CommandPlayerKickConfusion : public CommandPlayerActionAggressive
	{
	public:
		explicit CommandPlayerKickConfusion(CommandList* const parent);

		void onClick(Click& click) final;
	};
}
