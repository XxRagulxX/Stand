#pragma once

#include "Commands/Widgets/CommandListNotifySettings.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Rendering/Gui.hpp"

#include "Rendering/StandPort/CommandPosition2d.hpp"
#include "Rendering/StandPort/GridToaster.hpp"
#include "Rendering/StandPort/NotifyGrid.hpp"

namespace Stand
{
	class CommandListNotifySettings;

	class CommandNotifyType : public CommandListSelect
	{
	public:
		CommandPosition2d* custom_pos;

		explicit CommandNotifyType(CommandList* parent)
			: CommandListSelect(parent, LOC("TYP"), {}, NOLABEL, {
				{0, LOC("GME")},
				{1, LOC("STNDNTF_M")},
				{2, LOC("STNDNTF_C")},
			}, 1)
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
			if (g_gui.isUnloadPending())
			{
				return;
			}
			((CommandListNotifySettings*)parent)->setToaster(click.thread_context, &g_grid_toaster);
			g_notify_grid.next_to_map = (value <= 1);
			if (g_notify_grid.next_to_map)
			{
				g_notify_grid.origin.y = 1060;
				g_notify_grid.setOriginNextToMap();
				g_notify_grid.update();
			}
			else
			{
				g_notify_grid.origin = custom_pos->getPosition();
				if (g_notify_grid.invert_flow)
				{
					g_notify_grid.update();
				}
				else
				{
					g_notify_grid.updatePositions();
				}
			}
		}
	};
}
