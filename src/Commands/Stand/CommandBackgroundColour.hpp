#pragma once

#include "Commands/Widgets/CommandColour.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandBackgroundColour : public CommandColour
	{
	public:
		explicit CommandBackgroundColour(CommandList* const parent)
			: CommandColour(parent, LOC("CLRBR"), { CMDNAME("background") }, NOLABEL, 0, 0, 0, 77)
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				g_renderer.bgRectColour = getRGBA();
			}
		}
	};
}
