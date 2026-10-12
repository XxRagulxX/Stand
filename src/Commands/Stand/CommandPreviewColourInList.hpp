#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandPreviewColourInList : public CommandToggle
	{
	public:
		explicit CommandPreviewColourInList(CommandList* const parent)
			: CommandToggle(parent, LOC("PRCWCLRLST"), {}, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
			if (g_gui.canUpdateThemeSetting())
			{
			}
		}
	};
}
