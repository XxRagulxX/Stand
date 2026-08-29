#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/ScriptFuncPatch.hpp"

namespace Stand
{
	class CommandFullPlayerList : public CommandToggle
	{
	private:
		ScriptFuncPatch patch_1;
		ScriptFuncPatch patch_2;

	public:
		explicit CommandFullPlayerList(CommandList* const parent);

		void onEnable(Click& click) final;
	};
}
