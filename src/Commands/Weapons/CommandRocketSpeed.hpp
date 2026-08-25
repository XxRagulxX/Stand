#pragma once

#include "Commands/Widgets/CommandSliderFloat.hpp"

#include "Weapons/WeaponMgr.hpp"

namespace Stand
{
	class CommandRocketSpeed : public CommandSliderFloat
	{
	public:
		explicit CommandRocketSpeed(CommandList* const parent)
			: CommandSliderFloat(parent, LOC("RKSPDMUL"), { CMDNAME("rocketspeedmultiplier") }, NOLABEL, 0, 3000, 100, 10)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			ensureScriptThread(click, [this]
			{
				WeaponMgr::rocket_speed_multiplier = getFloatValue();
			});
		}
	};
}
