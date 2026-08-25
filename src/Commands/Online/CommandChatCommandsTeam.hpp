#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Network/ChatCommandsCommon.hpp"
#include "Network/evtChatEvent.hpp"

namespace Stand
{
	class CommandChatCommandsTeam : public CommandToggle
	{
	public:
		explicit CommandChatCommandsTeam(CommandList* const parent)
			: CommandToggle(parent, LOC("ON"))
		{
		}

		void onEnable(Click& click) final
		{
			evtChatEvent::registerHandler(&onChat);
		}

		void onDisable(Click& click) final
		{
			evtChatEvent::unregisterHandler(&onChat);
		}

	private:
		static void onChat(evtChatEvent& e)
		{
			if (e.team_chat)
			{
				ChatCommandsCommon::processMessageFromSenderWithChatCommandPermissions(e);
			}
		}
	};
}
