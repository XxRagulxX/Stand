#pragma once

#include "Rendering/StandPort/CommandColour.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandBlurTextColour : public CommandColour
	{
	public:
		explicit CommandBlurTextColour(CommandList* const parent)
			: CommandColour(parent, LOC("CLRBT"), { CMDNAME("unfocusedtext") }, NOLABEL, DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f))
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
				Rendering::Theme::kUnfocusedText = getRGBA();
			}
		}
	};
}
