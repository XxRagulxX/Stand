#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandSuppressGenericResponsesHotkey : public CommandToggle
	{
	public:
		explicit CommandSuppressGenericResponsesHotkey(CommandList* const parent)
			: CommandToggle(parent, LOC("HOTKEY"))
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
