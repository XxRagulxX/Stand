#pragma once

#include "Commands/Online/CommandSessionScript.hpp"

#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Game/natives.hpp"
#include "Scripting/ScriptGlobal.hpp"

#include "Commands/Online/CommandSessionScriptChallengesType.hpp"
#include "Commands/Online/CommandSessionScriptChallengesScore.hpp"

namespace Stand
{
	class CommandSessionScriptChallenges : public CommandSessionScript
	{
	public:
		explicit CommandSessionScriptChallenges(CommandList* const parent)
			: CommandSessionScript(parent, ATSTRINGHASH("AM_CHALLENGES"))
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
						*ScriptLocal(thread, LOCAL_CHLNG_SHDATA).as<int*>() = 3;
					}
				});
			}, CMDFLAGS_ACTION | CMDFLAG_TEMPORARY);
			this->createChild<CommandSessionScriptChallengesType>();
			this->createChild<CommandSessionScriptChallengesScore>();
			this->createChild<CommandLambdaAction>(LOC("STRTINST"), {}, NOLABEL, [this](Click& click)
			{
				if (!click.isUltimateEdition())
				{
					return;
				}
				click.ensureYieldableScriptThread([this]
				{
					if (auto thread = getThreadAndEnsureControl())
					{
						*ScriptLocal(thread, LOCAL_CHLNG_SHDATA + LOCAL_SHDATA_WARMUP).as<int*>() = NETWORK::GET_NETWORK_TIME() - ScriptGlobal(GLOBAL_TUNABLE_BASE + GLOBAL_TUNABLE_WARMUP_CHALLENGES).get<int>();
					}
				});
			}, CMDFLAGS_ACTION | CMDFLAG_TEMPORARY);
		}
	};
}
