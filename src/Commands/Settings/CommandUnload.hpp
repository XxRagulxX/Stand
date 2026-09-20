#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/common.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/Script.hpp"

namespace Stand
{
	class CommandUnload : public CommandPhysical
	{
	public:
		explicit CommandUnload(CommandList* parent) :
		    CommandPhysical(COMMAND_ACTION, parent, LIT("Unload Stand"), CMDNAMES("unload"), LIT("Removes Stand from the game until you re-inject it."))
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("Stand Enhanced", "Stand is being unloaded.", NotificationType::Info, 3000);
			Script::current()->yield(1500);
			g_Running = false;
		}
	};
}
