#pragma once

#include "Commands/Player/CommandPlayerActionAggressive.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Network/PlayerProvider.hpp"
#include "Network/script_events.hpp"
#include "Util/tse.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerCasinoTp : public CommandPlayerActionAggressive
	{
	public:
		explicit CommandPlayerCasinoTp(CommandList* const parent)
			: CommandPlayerActionAggressive(parent, LOC("PLYCASINOTP"), { CMDNAME("casinotp") })
		{
		}

		void onClick(Click& click) final
		{
			if (click.inOnline())
			{
				ensureScriptThread(click, [this]
				{
					DEF_P2;
					for (const AbstractPlayer p : pp->getPlayers(pp->single))
					{
						uint64_t args[] = {
							Util::i32_to_u64_trickery(SE_WARP_INTERIOR),
							g_player,
							Util::i32_to_u64_trickery(123),
							Util::i32_to_u64_trickery(0),
							Util::i32_to_u64_trickery(0),
							Util::i32_to_u64_trickery(1),
							Util::i32_to_u64_trickery(-1001291848),
							Util::i32_to_u64_trickery(-1016910157),
							Util::i32_to_u64_trickery(1108672448),
							Util::i32_to_u64_trickery(0),
							Util::i32_to_u64_trickery(-1),
							Util::i32_to_u64_trickery(0),
							Util::i32_to_u64_trickery(2147483647),
							Util::i32_to_u64_trickery(0),
							Util::i32_to_u64_trickery(-1)
						};
						TSE(1 << p, args);
					}
				});
			}
		}
	};
}
