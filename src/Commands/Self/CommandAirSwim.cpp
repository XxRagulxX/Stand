#include "Commands/LoopedCommand.hpp"
#include "Ped/PedConfigFlag.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandAirSwim: forces the "is
	// swimming" ped config flag on every tick, tricking the animation/
	// movement system into swim-style movement while airborne.
	class AirSwim : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			Self::GetPed().SetConfigFlag(PedConfigFlag::IsSwimming, true);
		}
	};

	static AirSwim _AirSwim{"airswim", "Air Swim", "Swim while in mid-air"};
}
