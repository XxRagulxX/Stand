#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class GracefulLanding : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		void OnTick() override
		{
			if (auto ped = Self::GetPed())
			{
				PED::SET_DISABLE_HIGH_FALL_DEATH(ped.GetHandle(), true);
				PED::SET_PED_CAN_RAGDOLL(ped.GetHandle(), false);
			}
		}

		void OnDisable() override
		{
			if (auto ped = Self::GetPed())
			{
				PED::SET_DISABLE_HIGH_FALL_DEATH(ped.GetHandle(), false);
				PED::SET_PED_CAN_RAGDOLL(ped.GetHandle(), true);
			}
		}
	};

	static GracefulLanding _GracefulLanding{"gracefullanding", "Graceful Landing", ""};
}
