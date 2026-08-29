#pragma once

#include "Commands/Player/CommandPlayerAction.hpp"

#include "Core/AbstractEntity.hpp"
#include "World/PlaceObjective.hpp"
#include "Core/TpUtil.hpp"

namespace Stand
{
	class CommandPlayerTpToMyObjective : public CommandPlayerAction
	{
	public:
		explicit CommandPlayerTpToMyObjective(CommandList* const parent)
			: CommandPlayerAction(parent, LOC("PLYSMMNOBJ"), CMDNAMES_OBF("summonobj", "summonobjective", "objectivesummon", "objsummon"))
		{
		}

		void onClick(Click& click) final
		{
			PlaceObjective objective;

			if (auto option = objective.getClosestPop(click); option.has_value())
			{
				const auto pos = option.value().pos;

				queueJob([this, pos]
				{
					for (const auto& p : PP_PTR->getPlayers())
					{
						if (p == g_player)
						{
							TpUtil::teleport(pos);
						}
						else
						{
							p.fiberTeleport(pos);
						}
					}
				});
			}
		}
	};
}