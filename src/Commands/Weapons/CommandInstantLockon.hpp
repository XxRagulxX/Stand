#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Weapons/WeaponMgr.hpp"

namespace Stand
{
	class CommandInstantLockon : public CommandToggle
	{
	public:
		explicit CommandInstantLockon(CommandList* parent)
			: CommandToggle(parent, LOC("INSTLKNON"), { CMDNAME("instantlockon") })
		{
		}

		void onEnable(Click& click) final
		{
			WeaponMgr::lockon_speed_multiplier = 0.0f;
		}

		void onDisable(Click& click) final
		{
			WeaponMgr::lockon_speed_multiplier = 1.0f;
		}
	};
}
