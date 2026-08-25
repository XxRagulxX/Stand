#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "lib/soup/Bytepatch.hpp"

namespace Stand
{
	class CommandForceRelay : public CommandToggle
	{
	private:
		soup::Bytepatch patch;
		
	public:
		explicit CommandForceRelay(CommandList* const parent);

		void onEnable(Click& click);
		void onDisable(Click& click);
	};
}
