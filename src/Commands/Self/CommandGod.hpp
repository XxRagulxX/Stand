#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/gta_entity.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandGod : public CommandToggle
	{
	public:
		explicit CommandGod(CommandList* const parent)
			: CommandToggle(parent, LOC("GOD"), { CMDNAME("godmode"), CMDNAME("immortality") }, LOC("GOD_H"))
		{
		}

		void onEnable(Click& click) final
		{
			g_gui.godmode = true;
		}

		void onDisable(Click& click) final
		{
			g_gui.godmode = false;
			ensureScriptThread(click, []
			{
				g_gui.onGodmodeDisable();
			});
		}
	};
}
