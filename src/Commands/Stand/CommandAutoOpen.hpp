#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandAutoOpen : public CommandToggle
	{
	public:
		inline static bool did_it = false;

		explicit CommandAutoOpen(CommandList* const parent)
			: CommandToggle(parent, LOC("INJOPEN2"))
		{
		}

		void onEnable(Click& click) final
		{
			if (!did_it)
			{
				if (click.type == CLICK_BULK)
				{
					g_gui.opened = true;
				}
				did_it = true;
			}
		}
	};
}
