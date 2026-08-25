#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/natives.hpp"

namespace Stand
{
	class CommandNoStuntJumps : public CommandToggle
	{
	public:
		explicit CommandNoStuntJumps(CommandList* const parent)
			: CommandToggle(parent, LOC("NSTNTJMP"), combineCommandNames(CMDNAMES("no", "block", "disable"), CMDNAME("stuntjumps")))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []
			{
				MISC::CANCEL_STUNT_JUMP();
			});
		}
	};
}
