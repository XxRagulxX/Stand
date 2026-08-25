#pragma once

#include "Commands/Player/CommandPlayerToggle.hpp"

#include "Network/script_events.hpp"
#include "Game/pointers.hpp"
#include "Util/tse.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerForceCamFront : public CommandPlayerToggle
	{
	public:
		explicit CommandPlayerForceCamFront(CommandList* const parent)
			: CommandPlayerToggle(parent, LOC("PLYFRCECAMFRNT"), { CMDNAME("confuse") }, LOC("PLYFRCECAMFRNT_H"), COMMANDPERM_AGGRESSIVE)
		{
		}

		void onEnable(Click& click) final
		{
			PC_PTR->ensureOnlineAndWarnSelfharm(this, click, [this](ThreadContext thread_context)
			{
				PP_PTR->toggleBatch(getPathEnFromParent(COMMAND_LIST_PLAYER), [](const std::vector<AbstractPlayer>& players)
				{
					for (AbstractPlayer p : players)
					{
						uint64_t args[] = {
							Util::i32_to_u64_trickery(SE_GAME),
							g_player,
							Util::i32_to_u64_trickery(SE_GAME_FORCECAMFRONT),
							0
						};
						TSE(1 << p, args);
					}
				});
			});
		}
	};
}
