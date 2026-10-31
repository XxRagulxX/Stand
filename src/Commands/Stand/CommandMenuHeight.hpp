#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandMenuHeight : public CommandSlider
	{
	public:
		explicit CommandMenuHeight(CommandList* const parent)
			: CommandSlider(parent, LOC("MNUHGT"), { CMDNAME("menuheight") }, NOLABEL, 3, 255, 11, 1)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			g_MenuGrid.updateSetting([this, thread_context{click.thread_context}]
			{
				Rendering::Theme::kMenuHeight = static_cast<uint8_t>(value);
				g_gui.command_rows = value;
				Rendering::GridItemListGrid::InvalidateAll();
				g_gui.getCurrentMenuFocusPhysical()->focusInParent(thread_context);
			});
		}
	};
}
