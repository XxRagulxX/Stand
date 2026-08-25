#pragma once

#include "Commands/Player/CommandPlayerActionAggressive.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Network/script_events.hpp"
#include "Util/tse.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerTriggerDefend : public CommandPlayerActionAggressive
	{
	public:
		explicit CommandPlayerTriggerDefend(CommandList* const parent)
			: CommandPlayerActionAggressive(parent, LOC("TRIGDEFD"), CMDNAMES_OBF("raid"))
		{
		}

		void onClick(Click& click) final
		{
			PC_PTR->ensureOnlineAndWarnSelfharm(this, click, [this](ThreadContext thread_context)
			{
				ensureScriptThread(thread_context, [this]
				{
					for (const auto& p : PP_PTR->getPlayers())
					{
						uint64_t args[] = {
							Util::i32_to_u64_trickery(SE_TRIGGER_DEFEND),
							g_player,
							0
						};
						TSE(1 << p, args);
					}
				});
			});
		}
	};
}
