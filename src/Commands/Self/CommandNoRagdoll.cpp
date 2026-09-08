#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class NoRagdoll : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			Self::GetPed().SetRagdoll(false);
			Self::GetPlayer().SetFallDistanceOverride(99999.0f);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			Self::GetPed().SetRagdoll(true);
			Self::GetPlayer().SetFallDistanceOverride(0.0f);
		}
	};

	static NoRagdoll _NoRagdoll{"noragdoll", "No Ragdoll", "Prevents your ped from ragdolling"};
}