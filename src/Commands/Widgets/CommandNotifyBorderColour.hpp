#pragma once

#include "Rendering/StandPort/CommandColour.hpp"

#include "Rendering/StandPort/StandRendererCompat.hpp"

namespace Stand
{
	class CommandNotifyBorderColour : public CommandColour
	{
	public:
		explicit CommandNotifyBorderColour(CommandList* parent, CommandColourCustom* with_copy_from)
			: CommandColour(parent, LOC("BRDCLR"), { CMDNAME("notifyborder") }, NOLABEL, {1.0f, 0.0f, 1.0f, 1.0f}, true, CMDFLAGS_COLOUR, with_copy_from)
		{
		}

		void onChange(Click& click) final
		{
			g_renderer.notifyBorderColour = getRGBA();
		}
	};
}
