#pragma once

#include "Commands/Player/CommandPlayerToggle.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Localization/lang.hpp"
#include "Game/natives.hpp"
#include "Network/PlayerProvider.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerFreeze : public CommandPlayerToggle
	{
	public:
		explicit CommandPlayerFreeze(CommandList* const parent)
			: CommandPlayerToggle(parent, LOC("FRZ"), { CMDNAME("freeze") }, NOLABEL, COMMANDPERM_AGGRESSIVE)
		{
		}

		void onEnable(Click& click) final
		{
			PP_PTR->toggleBatch(getPathEnFromParent(COMMAND_LIST_PLAYER), [](const std::vector<AbstractPlayer>& players)
			{
				for (const AbstractPlayer& p : players)
				{
					p.sendFreeze();
				}
			});
		}
	};
}
