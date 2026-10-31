#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/HeaderBanner.hpp"

namespace Stand
{
	class CommandHeaderBgBlur : public CommandToggle
	{
	public:
		explicit CommandHeaderBgBlur(CommandList* const parent)
			: CommandToggle(parent, LOC("BGBLR"), { CMDNAME("headerbgblur") }, LOC("HDR_BGBLR_H"), false)
		{
		}

		void onChange(Click& click) override
		{
			if (g_gui.canUpdateThemeSetting())
				Rendering::HeaderBanner::SetBgBlur(m_on);
		}
	};
}
