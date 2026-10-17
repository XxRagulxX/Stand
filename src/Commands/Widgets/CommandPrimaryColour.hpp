#pragma once

#include "Rendering/StandPort/CommandColour.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandPrimaryColour : public CommandColour
	{
	public:
		explicit CommandPrimaryColour(CommandList* const parent)
			: CommandColour(parent, LOC("CLRFR"), { CMDNAME("primary") }, NOLABEL, 255, 0, 255, 255)
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				Rendering::Theme::kAccent = getRGBA();
			}
		}
	};
}
