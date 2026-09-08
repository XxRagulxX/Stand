#include "Commands/CommandLegacy.hpp"
#include "Scripting/Natives.hpp"
#include "Rendering/Notifications.hpp"

namespace Stand::Features
{
	class ForceQuitToStoryMode : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (NETWORK::NETWORK_IS_SESSION_ACTIVE())
			{
				try
				{
					NETWORK::SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE();
				}
				catch (const std::exception& e)
				{
					LOG(FATAL) << "Transition failed: " << e.what();
				}
			}
			else
			{
				Notifications::Show("Force Quit to Story Mode", "Already in Story Mode");
			}
		}
	};

	static ForceQuitToStoryMode _ForceQuitToStoryMode{"forcequittosp", "Force Quit to SP", "Go to single player story mode"};
}
