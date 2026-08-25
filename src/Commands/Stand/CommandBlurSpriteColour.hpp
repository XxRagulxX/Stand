#pragma once

#include "Commands/Widgets/CommandColour.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandBlurSpriteColour : public CommandColour
	{
	public:
		explicit CommandBlurSpriteColour(CommandList* const parent, CommandColour* with_copy_from)
			: CommandColour(parent, LOC("CLRBS"), { CMDNAME("unfocusedtexture") }, NOLABEL, DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f), true, CMDFLAGS_COLOUR, with_copy_from)
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				g_renderer.bgSpriteColour = getRGBA();
			}
		}
	};
}
