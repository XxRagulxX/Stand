#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandMouse : public CommandToggle
	{
	public:
		explicit CommandMouse(CommandList* const parent)
			: CommandToggle(parent, LOC("MOUSE"), CMDNAMES("mouse"), LOC("MOUSE_H"))
		{
		}

		void onEnable(Click& click) final
		{
			if (click.isAuto())
			{
				g_gui.mouse_mode = MouseMode::NAVIGATE;
			}
		}

		void onDisable(Click& click) final
		{
			g_gui.mouse_mode = MouseMode::NONE;
		}
	};
}
