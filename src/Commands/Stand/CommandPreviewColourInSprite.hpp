#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandPreviewColourInSprite : public CommandToggle
	{
	public:
		explicit CommandPreviewColourInSprite(CommandList* const parent)
			: CommandToggle(parent, LOC("PRVWCLRTXR"), {}, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				g_renderer.preview_colour_in_sprite = m_on;
			}
		}
	};
}
