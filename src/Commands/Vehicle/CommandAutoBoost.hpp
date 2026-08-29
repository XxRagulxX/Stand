#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/atStringHash.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	class CommandAutoBoost : public CommandToggle
	{
	public:
		explicit CommandAutoBoost(CommandList* const parent)
			: CommandToggle(parent, LOC("AUTORKTBST"), { CMDNAME("autoboost") }, LOC("AUTORKTBST_H2"))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []()
			{
				auto veh = g_player_veh;
				if (veh.isValid()
					&& veh.isOwner()
					)
				{
					VEHICLE::SET_ROCKET_BOOST_ACTIVE(veh, true);
				}
			});
		}
	};
}
