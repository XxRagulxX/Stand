#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class GhostMode : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			PED::SET_PED_CAPSULE(Self::GetPed().GetHandle(), 1.175494e-38f);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			PED::SET_PED_CAPSULE(Self::GetPed().GetHandle(), 0.25f);
		}
	};

	static GhostMode _GhostMode{"ghostmode", "Reduced Collision", "Shrinks your collision so you can pass through gaps"};
}
