#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

#include "Menu/MenuGrid.hpp"

namespace Stand
{
	class CommandInfoTextPadding : public CommandSlider
	{
	public:
		explicit CommandInfoTextPadding(CommandList* parent)
			: CommandSlider(parent, LOC("PDDGN"), { CMDNAME("infopadding") }, NOLABEL, -100, 100, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			g_menu_grid.updateSetting([this]
			{
				g_menu_grid.info_padding = value;
			});
		}
	};
}
