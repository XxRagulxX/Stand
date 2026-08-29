#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandCursorPosIncludesDividers : public CommandToggle
	{
	public:
		explicit CommandCursorPosIncludesDividers(CommandList* const parent)
			: CommandToggle(parent, LOC("CRSPOSIDIV"))
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				g_gui.cursor_pos_includes_dividers = m_on;
			}
		}
	};
}
