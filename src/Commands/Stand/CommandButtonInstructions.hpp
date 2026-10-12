#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	enum class ButtonInstructionsMode : uint8_t
	{
		ALWAYS,
		SMART,
		NEVER,
	};

	class CommandButtonInstructions : public CommandListSelect
	{
	public:
		explicit CommandButtonInstructions(CommandList* const parent)
			: CommandListSelect(parent, LOC("SHWBI"), { CMDNAME("showbuttoninstructions") }, NOLABEL, {
				{(long long)(ButtonInstructionsMode::ALWAYS), LOC("ON")},
				{(long long)(ButtonInstructionsMode::SMART), LOC("SMRT")},
				{(long long)(ButtonInstructionsMode::NEVER), LOC("DOFF")},
			}, (long long)(ButtonInstructionsMode::ALWAYS))
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
		}
	};
}
