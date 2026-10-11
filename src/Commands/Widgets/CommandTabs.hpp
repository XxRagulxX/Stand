#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/StandPort/Direction.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"
#include "Rendering/StandPort/StandRendererCompat.hpp"

namespace Stand
{
	class CommandTabs : public CommandToggle
	{
	public:
		CommandListSelect* pos;

		explicit CommandTabs(CommandList* const parent)
			: CommandToggle(parent, LOC("TABS"), CMDNAMES("tabs"), NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				g_renderer.tabs_pos = m_on ? (Direction)pos->value : NONE;
				g_gui.updateTabs(click.thread_context);
				g_MenuGrid.updateSetting([] {});
			}
		}
	};
}
