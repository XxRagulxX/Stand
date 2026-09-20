#include "Commands/LoopedCommand.hpp"
#include "Scripting/Globals.hpp"
#include "Core/Pointers.hpp"

namespace Stand::Features
{
	class SilencePhonecalls : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			using namespace Globals;

			if (*Pointers.IsSessionStarted)
			{
				if (*PHONE_CALL_STATE.as<int*>() != 0 && *PHONE_CALL_STATE.as<int*>() != 5 && *PHONE_CALL_STATE.as<int*>() != 6
				    && *PHONE_CALL_IN_PROGRESS.as<bool*>() && *PHONE_INCOMING_CALL.as<bool*>())
				{
					LOGF(VERBOSE, "SilencePhoneCalls::OnTick(): Skipped phone call from character {}", *PHONE_CALLING_CHARACTER.as<int*>());
					*PHONE_CALL_STATE.as<int*>() = 6;
				}
			}
		}
	};

	static SilencePhonecalls _SilencePhonecalls{"nocalls", "Silence Phone Calls", "Automatically silences all incoming calls"};
}
