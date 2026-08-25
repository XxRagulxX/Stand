#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Core/AllEntitiesEveryTick.hpp"
#include "Network/punishments.hpp"

namespace Stand
{
	class CommandPlayerAimPunishment : public CommandToggle
	{
	private:
		const punishment_t mask;

	public:
		explicit CommandPlayerAimPunishment(CommandList* const parent, const Punishment& p)
			: CommandToggle(parent, Label(p.name), {}, Label(p.help_text)), mask(p.mask)
		{
		}

		void onEnable(Click& click) final
		{
			AllEntitiesEveryTick::player_aim_punishments |= mask;
		}

		void onDisable(Click& click) final
		{
			AllEntitiesEveryTick::player_aim_punishments &= ~mask;
			if (mask == PUNISHMENT_FREEZE)
			{
				ensureScriptThread(click, []
				{
					for (const auto& p : AbstractPlayer::listAll())
					{
						p.unfreeze();
					}
				});
			}
		}
	};
}
