#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class AllowHatsInVehicles : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			PED::SET_PED_RESET_FLAG(Self::GetPed().GetHandle(), 337, true);
		}
	};

	static AllowHatsInVehicles _AllowHatsInVehicles{"allowhatsinvehicles", "Allow Hats In Vehicles", "Allows you to keep your hats or head gear inside vehicles."};
};
