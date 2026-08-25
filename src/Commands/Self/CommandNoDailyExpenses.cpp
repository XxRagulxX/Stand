#include "Commands/Self/CommandNoDailyExpenses.hpp"

#include "lib/soup/macros.hpp"
#include "Game/script_func.hpp"
#include "Scripting/ScriptVmOpCode.hpp"

namespace Stand
{
	CommandNoDailyExpenses::CommandNoDailyExpenses(CommandList* parent)
		: CommandToggle(parent, LOC("NDLYXPS"), { CMDNAME("nodailyexpenses") })
	{
	}

	void CommandNoDailyExpenses::onEnable(Click& click)
	{
		registerScriptTickEventHandler(click, [this]
		{
			if (m_on)
			{
				patch.onTick(ATSTRINGHASH("freemode"), FUNC_FM_DAILY_EXPENSES, ScriptFuncPatch::patchBytesReturnVoid_0, COUNT(ScriptFuncPatch::patchBytesReturnVoid_0));
				return true;
			}
			patch.onDisable(ATSTRINGHASH("freemode"));
			return false;
		});
	}
}
