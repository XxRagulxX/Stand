#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/ScriptFuncPatch.hpp"

namespace Stand
{
	class CommandNoDailyExpenses : public CommandToggle
	{
	private:
		ScriptFuncPatch patch;

	public:
		explicit CommandNoDailyExpenses(CommandList* const parent);

		void onEnable(Click& click) final;
	};
}
