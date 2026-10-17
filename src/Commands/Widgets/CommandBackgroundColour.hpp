#pragma once

#include "Rendering/StandPort/CommandColour.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Theme.hpp"

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
				Rendering::Theme::kPanelBackground = getRGBA();
			}
		}
	};
}
