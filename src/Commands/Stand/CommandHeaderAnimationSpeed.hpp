#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Renderer.hpp"

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
				g_renderer.header_speed = value;
			}
		}
	};
}
