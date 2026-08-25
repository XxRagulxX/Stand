#pragma once

#include "Commands/Player/CommandPlayerToggle.hpp"

namespace Stand
{
	class CommandPlayerTimeout : public CommandPlayerToggle
	{
	public:
		explicit CommandPlayerTimeout(CommandList* const parent);

		void onEnable(Click& click) final;
		void onDisable(Click& click) final;
	};
}
