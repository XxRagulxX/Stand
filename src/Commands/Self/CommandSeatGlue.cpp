#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand's own CommandSeatglue just flips a g_gui.seatglue bool
	// that some other, central per-tick file (not CommandTabSelf.cpp,
	// the only real Stand source this project has ever had access to -
	// see this file's own sibling commands' comments) reads to keep you
	// seated. Rather than guess at that file's exact mechanism, this uses
	// the same native our own Ped::SetRagdoll() already leans on for its
	// "don't get knocked off a vehicle" half (see Ped.cpp) - called
	// directly here, independent of ragdoll state, so this is its own
	// toggle rather than a side effect of Gracefulness/No Ragdoll.
	class SeatGlue : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Self::GetPed().GetHandle(), false);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Self::GetPed().GetHandle(), true);
		}
	};

	static SeatGlue _SeatGlue{"seatglue", "Glued To Seats", "Stops you being knocked out of a vehicle's seat"};
}
