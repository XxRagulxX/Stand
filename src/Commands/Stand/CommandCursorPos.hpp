#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Menu/MenuGrid.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandCursorPos : public CommandToggle
	{
	public:
		explicit CommandCursorPos(CommandList* const parent)
			: CommandToggle(parent, LOC("SHWCRSPOS"), CMDNAMES("cursorpos", "showcursorpos"), LOC("SHWCRSPOS_H"))
		{
		}

		void onChange(Click& click) final
		{
			g_menu_grid.updateSetting([this]
			{
				g_gui.show_cursor_pos = m_on;
			});
		}
	};
}
