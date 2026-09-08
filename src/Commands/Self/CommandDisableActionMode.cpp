#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class DisableActionMode : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			PED::SET_PED_RESET_FLAG(Self::GetPed().GetHandle(), 200, true);
		}
	};

	static DisableActionMode _DisableActionMode{"disableactionmode", "Disable Action Mode", "Disables the player's janky movement during combat."};
};
