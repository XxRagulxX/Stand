#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Network/ChatCommandsCommon.hpp"
#include "Network/evtChatEvent.hpp"

namespace Stand
{
	class CommandChatCommandsStrangers : public CommandToggle
	{
	public:
		explicit CommandChatCommandsStrangers(CommandList* const parent)
			: CommandToggle(parent, LOC("ON"), { CMDNAME("commandsforstrangers"), CMDNAME("chatcommandsforstrangers") })
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
			if (e.sender != g_player
				&& !AbstractPlayer(e.sender).isFriend()
				&& !AbstractPlayer(e.sender).isCrewMember()
				)
			{
				ChatCommandsCommon::processMessageFromSenderWithChatCommandPermissions(e);
			}
		}
	};
}
