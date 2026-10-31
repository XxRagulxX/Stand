#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/HeaderBanner.hpp"

namespace Stand
{
	class CommandHeaderAnimationSpeed : public CommandSlider
	{
	public:
		explicit CommandHeaderAnimationSpeed(CommandList* parent)
			: CommandSlider(parent, LOC("FRMINT"), { CMDNAME("headerinterval") }, NOLABEL, 1, 10000, 32)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				Rendering::HeaderBanner::SetFrameIntervalMs(static_cast<int64_t>(value));
			}
		}
	};
}
