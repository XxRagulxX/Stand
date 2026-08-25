#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/ScriptFuncPatch.hpp"

namespace Stand
{
	class CommandBypassTimeTrialUnsuitable : public CommandToggle
	{
	private:
		ScriptFuncPatch patch;

	public:
		explicit CommandBypassTimeTrialUnsuitable(CommandList* const parent);

		void onEnable(Click& click) final;
	};
}
