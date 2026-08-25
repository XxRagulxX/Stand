#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/Hooking.hpp"

namespace Stand
{
	class CommandGracefulLanding : public CommandToggle
	{
	public:
		explicit CommandGracefulLanding(CommandList* const parent)
			: CommandToggle(parent, LOC("GRCELND"), { CMDNAME("gracefullanding"), CMDNAME("gracefulanding"), CMDNAME("graceland") })
		{
		}
		
		void onEnable(Click& click) final
		{
			++g_hooking.graceful_landing;
		}

		void onDisable(Click& click) final
		{
			--g_hooking.graceful_landing;
		}
	};
}
