#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandKeepCursorRoot : public CommandToggle
	{
	public:
		explicit CommandKeepCursorRoot(CommandList* const parent)
			: CommandToggle(parent, LOC("TABS"), { CMDNAME("tabskeepcursor"), CMDNAME("tabkeepscursor") }, LOC("KPCRSRR_H"), true)
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
