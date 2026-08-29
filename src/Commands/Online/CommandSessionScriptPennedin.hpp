#pragma once

#include "Commands/Online/CommandSessionScript.hpp"

#include "Commands/Widgets/CommandLambdaAction.hpp"

#include "Commands/Online/CommandSessionScriptPennedinCagedin.hpp"

namespace Stand
{
	class CommandSessionScriptPennedin : public CommandSessionScript
	{
	public:
		explicit CommandSessionScriptPennedin(CommandList* const parent)
			: CommandSessionScript(parent, ATSTRINGHASH("AM_PENNED_IN"))
		{
			this->createChild<CommandLambdaAction>(LOC("LUA_STOP"), {}, NOLABEL, [this](Click& click)
			{
				if (!click.isUltimateEdition())
				{
					return;
				}
				click.ensureYieldableScriptThread([this]
				{
					if (auto thread = getThreadAndEnsureControl())
					{
						*ScriptLocal(thread, LOCAL_PNDIN_SHDATA).as<int*>() = 4;
					}
				});
			}, CMDFLAGS_ACTION | CMDFLAG_TEMPORARY);
			this->createChild<CommandSessionScriptPennedinCagedin>();
		}
	};
}
