#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Network/evtChatEvent.hpp"
#include "Core/ConsoleLogger.hpp"

namespace Stand
{
	class CommandChatLogConsole : public CommandToggle
	{
	private:
		static void onChatEvent(evtChatEvent& e)
		{
			g_console.log(e.getLogEntry());
		}

	public:
		explicit CommandChatLogConsole(CommandList* const parent)
			: CommandToggle(parent, LOC("CON"))
		{
		}

		void onEnable(Click& click) final
		{
			evtChatEvent::registerHandler(&onChatEvent);
		}

		void onDisable(Click& click) final
		{
			evtChatEvent::unregisterHandler(&onChatEvent);
		}
	};
}
