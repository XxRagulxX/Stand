#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Network/is_session.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	class CommandTrnsSpdupFmmc : public CommandToggle
	{
	public:
		explicit CommandTrnsSpdupFmmc(CommandList* const parent)
			: CommandToggle(parent, LOC("TRNS_SPDUP_FMMC"), { CMDNAME("speedupfmmc") })
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []
			{
				if (is_session_transition_active(false))
				{
					*ScriptGlobal(g_sTransitionSessionData_bFmmcLauncherReady).as<BOOL*>() = TRUE;
				}
			});
		}
	};
}
