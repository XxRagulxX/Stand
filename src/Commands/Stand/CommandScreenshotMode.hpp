#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/tbScreenshotMode.hpp"

namespace Stand
{
	class CommandScreenshotMode : public CommandToggle
	{
	public:
		explicit CommandScreenshotMode(CommandList* const parent)
			: CommandToggle(parent, LOC("SCRNSHT"), { CMDNAME("screenshot") })
		{
		}

		void onEnable(Click& click) final
		{
			g_tb_screenshot_mode.enable();
		}

		void onDisable(Click& click) final
		{
			g_tb_screenshot_mode.disable();
		}
	};
}
