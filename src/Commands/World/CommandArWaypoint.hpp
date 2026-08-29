#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/DrawUtil3d.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandArWaypoint : public CommandToggle
	{
	public:
		explicit CommandArWaypoint(CommandList* const parent)
			: CommandToggle(parent, LOC("ARWP"), { CMDNAME("arwaypoint"), CMDNAME("arwp") }, LOC("ARWP_H"))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []()
			{
				if (!g_gui.waypoint.isNull()
					&& !DrawUtil3d::exclusive_ar
					)
				{
					DrawUtil3d::draw_ar_beacon(g_gui.waypoint);
				}
			});
		}
	};
}
