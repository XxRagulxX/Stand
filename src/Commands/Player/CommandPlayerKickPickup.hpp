#pragma once

#include "Commands/Player/CommandPlayerActionAggressive.hpp"

#include "Commands/Player/CommandListPlayer.hpp"
#include "Network/PlayerProvider.hpp"

namespace Stand
{
	class CommandPlayerKickPickup : public CommandPlayerActionAggressive
	{
	public:
		explicit CommandPlayerKickPickup(CommandList* const parent)
			: CommandPlayerActionAggressive(parent, LOC("PLYKCK_IPUP"), CMDNAMES_OBF("pickupkick"), LOC("PLYKCK_H_B_M"))
		{
		}

		void onClick(Click& click) final
		{
			PC_PTR->ensureOnlineAndWarnSelfharm(this, click, [this](ThreadContext thread_context)
			{
				ensureScriptThread(thread_context, [this]
				{
					PP_PTR->getPlayers(true).at(0).kickPickup();
				});
			});
		}
	};
}
