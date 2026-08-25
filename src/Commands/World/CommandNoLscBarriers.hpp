#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/ScriptFuncPatch.hpp"

namespace Stand
{
	class CommandNoLscBarriers : public CommandToggle
	{
	private:
		ScriptFuncPatch patch_1;
		ScriptFuncPatch patch_2;

	public:
		explicit CommandNoLscBarriers(CommandList* parent);

		void onEnable(Click& click) final;
	};
}
