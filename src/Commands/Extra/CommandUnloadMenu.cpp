#include "Commands/CommandLegacy.hpp"

namespace Stand::Features
{
	class UnloadMenu : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			g_Running = false;
		}
	};

	static UnloadMenu _UnloadMenu{"unloadmenu", "Unload", "Unload StandEnhanced."};
}