#include "Commands/Online/CommandSessionThunder.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Extra/CommandLambdaActionScript.hpp"
#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "Core/GlobalsWatcher.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	CommandSessionThunder::CommandSessionThunder(CommandList* const parent)
		: CommandList(parent, LOC("THDR"))
	{
		createChild<CommandLambdaActionScript>(LOC("THDR_ON"), CMDNAMES("thunderon"), NOLABEL, [](const Click& click)
		{
			if (click.inOnline())
			{
				*ScriptGlobal(GLOBAL_PLAYERREMOTE_BASE).at(g_player, GLOBAL_PLAYERREMOTE_SIZE).at(GLOBAL_PLAYERREMOTE_ORG).at(GLOBAL_ORG_MHBITSET).at(0, 1).as<int*>() |= (1 << eMH_MODEL_HIDE_BITSET_CrashSite2_activate_weather);
			}
		});
		createChild<CommandLambdaActionScript>(LOC("THDR_OFF"), CMDNAMES("thunderoff"), NOLABEL, [](const Click& click)
		{
			if (click.inOnline())
			{
				*ScriptGlobal(GLOBAL_PLAYERREMOTE_BASE).at(g_player, GLOBAL_PLAYERREMOTE_SIZE).at(GLOBAL_PLAYERREMOTE_ORG).at(GLOBAL_ORG_MHBITSET).at(0, 1).as<int*>() &= ~(1 << eMH_MODEL_HIDE_BITSET_CrashSite2_activate_weather);
			}
		});
		createChild<CommandTogglePointer>(&GlobalsWatcher::notify_thunder_request, LOC("THDR_N"), CMDNAMES("thundernotify"));
	}
}
