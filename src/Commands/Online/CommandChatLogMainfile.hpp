#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Network/evtChatEvent.hpp"
#include "Core/FileLogger.hpp"

namespace Stand
{
	class CommandChatLogMainfile : public CommandToggle
	{
	private:
		static void onChatEvent(evtChatEvent& e)
		{
			g_logger.log(e.getLogEntry());
		}

	public:
		explicit CommandChatLogMainfile(CommandList* const parent)
			: CommandToggle(parent, LIT("Log.txt"))
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
