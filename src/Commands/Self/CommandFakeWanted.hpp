#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
	class CommandFakeWanted : public CommandSlider
	{
	public:
		explicit CommandFakeWanted(CommandList* const parent)
			: CommandSlider(parent, LIT("Fake Wanted Level"), { CMDNAME("fakewanted") }, LIT("Shows fake wanted stars on the HUD without actually being wanted."), 0, 6, 0)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			if (value == 0)
			{
				CommandTickDispatch::RemoveCommand(this);
				MISC::SET_FAKE_WANTED_LEVEL(0);
			}
			else
			{
				MISC::SET_FAKE_WANTED_LEVEL(value);
				CommandTickDispatch::AddCommand(this);
			}
		}

		void onTick() override
		{
			MISC::SET_FAKE_WANTED_LEVEL(value);
		}

		~CommandFakeWanted() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
