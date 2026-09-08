#include "Commands/CommandListSelect.hpp"
#include "Ped/PedConfigFlag.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand's own 3-way list: Off (no-op), Full (always have a
	// parachute - the same thing this project's own, separate
	// "infiniteparachutes" toggle already does, see
	// CommandInfiniteParachutes.cpp), Empty (force the parachute weapon
	// and reserve flag off). Applied once, on change, rather than every
	// tick like Stand's own Full mode does - Empty is a one-shot removal
	// anyway, and Full here just mirrors GiveWeapon()'s own idempotent
	// "give if missing" shape, so re-applying every tick wouldn't do
	// anything Full mode's own re-pickup logic doesn't already cover for
	// the common case (something else forcibly clearing your parachute
	// won't get immediately corrected, the same disclosed gap as this
	// file's sibling commands here).
	class Paralock : public CommandListSelect
	{
		using CommandListSelect::CommandListSelect;

		virtual void OnChange() override
		{
			if (!Self::GetPed())
				return;

			switch (GetState())
			{
			case 1: // Full
				PLAYER::SET_PLAYER_HAS_RESERVE_PARACHUTE(Self::GetPlayer().GetId());
				if (!Self::GetPed().HasWeapon("GADGET_PARACHUTE"_J))
					Self::GetPed().GiveWeapon("GADGET_PARACHUTE"_J);
				break;

			case 2: // Empty
				Self::GetPed().RemoveWeapon("GADGET_PARACHUTE"_J);
				Self::GetPed().SetConfigFlag(PedConfigFlag::HasReserveParachute, false);
				break;

			default: // Off
				break;
			}
		}
	};

	static Paralock _Paralock{"paralock",
	    "Paralock",
	    "Force your parachute availability on or off",
	    {{0, "Off"}, {1, "Full"}, {2, "Empty"}},
	    0};
}
