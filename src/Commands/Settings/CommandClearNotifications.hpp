#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Notifications.hpp"

namespace Stand
{
	class CommandClearNotifications : public CommandPhysical
	{
	public:
		explicit CommandClearNotifications(CommandList* parent) :
		    CommandPhysical(COMMAND_ACTION, parent, LIT("Clear Notifications"), CMDNAMES("clearstandnotifys"), LIT("Clears Stand's notification system."))
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Clear();
		}
	};
}
