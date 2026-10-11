#pragma once

#include "Rendering/StandPort/CommandPosition2d.hpp"

#include "Rendering/StandPort/NotifyGrid.hpp"

namespace Stand
{
	class CommandNotifyPosition : public CommandPosition2d
	{
	public:
		explicit CommandNotifyPosition(CommandList* parent)
			: CommandPosition2d(parent, LOC("CUSTPOS"), CMDNAME("notify"), { 32, 532 }, LOC("CUSTPOS_H"))
		{
		}

		void onClick(Click& click) final
		{
			if (g_notify_grid.next_to_map)
			{
				click.setResponse(LOC("CUSTPOS_H"));
			}
			return CommandList::onClick(click);
		}

		void onChange(Rendering::Position2d&& pos) const final
		{
			if (!g_notify_grid.next_to_map)
			{
				g_notify_grid.origin = std::move(pos);
				g_notify_grid.updatePositions();
			}
		}
	};
}
