#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Vehicle/PlateDj.hpp"

namespace Stand
{
	class CommandPlateTps : public CommandToggle
	{
	public:
		explicit CommandPlateTps(CommandList* const parent)
			: CommandToggle(parent, LOC("TS"), { CMDNAME("platetps"), CMDNAME("platefps") }, LOC("TS_H"))
		{
		}

		void onEnable(Click& click) final
		{
			g_plate_dj.enableTrack(PlateDj::TPS);
		}

		void onDisable(Click& click) final
		{
			g_plate_dj.disableTrack(PlateDj::TPS);
		}
	};
}
