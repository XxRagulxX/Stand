#pragma once

#include "Commands/Player/CommandPlayerAction.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Network/PlayerProvider.hpp"
#include "Network/script_events.hpp"
#include "Util/tse.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerSendtojob : public CommandPlayerAction
	{
	private:
		const uint64_t value;

	public:
		explicit CommandPlayerSendtojob(CommandList* const parent, const uint64_t value, Label&& menu_name, std::vector<CommandName>&& command_names = {})
			: CommandPlayerAction(parent, std::move(menu_name), std::move(command_names)), value(value)
		{
		}

		void onClick(Click& click) final
		{
			PC_PTR->ensureOnlineAndWarnSelfharm(this, click, [this](ThreadContext thread_context)
			{
				ensureScriptThread(thread_context, [this]
				{
					DEF_P2;
					for (const AbstractPlayer p : pp->getPlayers(pp->single))
					{
						uint64_t args[] = {
							Util::i32_to_u64_trickery(SE_SEND_TO_JOB),
							g_player,
							value
						};
						TSE(1 << p, args);
					}
				});
			});
		}
	};
}
