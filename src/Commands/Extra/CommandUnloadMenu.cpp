#include "Commands/CommandLegacy.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/Script.hpp"

namespace Stand::Features
{
	class UnloadMenu : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		void OnCall() override
		{
			Notifications::Show(
			    "Stand Enhanced",
			    "Stand is being unloaded.",
			    NotificationType::Info,
			    3000);
			Script::current()->yield(1500);
			g_Running = false;
		}
	};

	static UnloadMenu _UnloadMenu{"unload", "Unload Stand", "Removes Stand from the game until you re-inject it."};
}
