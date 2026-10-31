#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandKeepCursorBack : public CommandToggle
	{
	public:
		explicit CommandKeepCursorBack(CommandList* const parent)
			: CommandToggle(parent, LOC("LSTS"), { CMDNAME("listskeepcursor"), CMDNAME("listkeepscursor") }, LOC("KPCRSRS_H"))
		{
		}

		void onChange(Click& click) final
		{
		}
	};
}
