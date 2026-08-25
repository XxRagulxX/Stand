#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Scripting/ScriptLocal.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Game/script_thread.hpp"

namespace Stand
{
	class CommandDisableNanoDroneAbilityCooldowns : public CommandToggle
	{
	public:
		explicit CommandDisableNanoDroneAbilityCooldowns(CommandList* const parent)
			: CommandToggle(parent, LOC("NONANOCLDNS"), CMDNAMES_OBF("no", "block", "disable", "bypass"))
		{
			addSuffixToCommandNames(CMDNAME_OBF("nanodroneabilitycooldowns"));
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []
			{
				if (!is_session_started())
				{
					return;
				}

				ScriptGlobal(GLOBAL_NANO_DRONE_NET_TIMER).set(0);

				if (auto script = GtaThread::fromHash(ATSTRINGHASH("am_mp_drone")))
				{
					// Shock Cooldown
					*ScriptLocal(script, LOCAL_DRONE_SHOCK_COOLDOWN).as<int*>() = 999999999;

					// Detonate Time
					*ScriptLocal(script, LOCAL_DRONE_DETONATE_COOLDOWN).as<int*>() = 999999999;

					// Boost Recharge Time
					*ScriptLocal(script, LOCAL_DRONE_BOOST_RECHARGE_COOLDOWN).as<int*>() = 999999999;
				}
			});
		}
	};
}
