#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class MobileRadio : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;


		virtual void OnTick() override
		{
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}

		virtual void OnDisable() override
		{
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(false);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	};

	static MobileRadio _MobileRadio{"mobileradio", "Mobile Radio", "Allows you to listen to any radio station on foot with Hotkey Q"};
}
