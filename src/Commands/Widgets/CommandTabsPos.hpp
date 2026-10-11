#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/StandPort/Direction.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"
#include "Rendering/StandPort/StandRendererCompat.hpp"

namespace Stand
{
	class CommandTabsPos : public CommandListSelect
	{
	private:
		CommandToggle* const tabs;

	public:
		explicit CommandTabsPos(CommandList* const parent, CommandToggle* tabs)
			: CommandListSelect(parent, LOC("POS"), { CMDNAME("tabsposition") }, NOLABEL, {
				{LEFT,   LOC("RPOS_L"), CMDNAMES("left")},
				{RIGHT,  LOC("RPOS_R"), CMDNAMES("right")},
				{TOP,    LOC("RPOS_T"), CMDNAMES("top")},
				{BOTTOM, LOC("RPOS_B"), CMDNAMES("bottom")},
			}, LEFT), tabs(tabs)
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
			if (tabs->m_on)
			{
				g_MenuGrid.updateSetting([this]
				{
					g_renderer.tabs_pos = (Direction)value;
				});
			}
		}
	};
}
