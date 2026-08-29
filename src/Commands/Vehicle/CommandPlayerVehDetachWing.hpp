#pragma once

#include "Commands/Vehicle/CommandPlayerVehicleAction.hpp"

#include "Game/eAircraftSections.hpp"
#include "Game/pointers.hpp"

namespace Stand
{
	class CommandPlayerVehDetachWing : public CommandPlayerVehicleAction
	{
	public:
		explicit CommandPlayerVehDetachWing(CommandList* parent)
			: CommandPlayerVehicleAction(parent, LOC("DETACH_AIR"), CMDNAMES("detachwing"), NOLABEL, PV_CONTROL | PV_ALLOWSELF, COMMANDPERM_AGGRESSIVE)
		{
		}

		void onClickImpl(AbstractPlayer p, AbstractEntity& veh, const Click& click) const final
		{
			if (veh.getModel().isPlane())
			{
				const auto plane = (CPlane*)veh.getCVehicle();
				pointers::CAircraftDamageBase_BreakOffSection(&plane->m_aircraftDamage, plane, soup::rand.coinflip() ? eAircraftSection::WING_L : eAircraftSection::WING_R, false, true);
				TASK::CLEAR_VEHICLE_CRASH_TASK(veh);
			}
			else
			{
				click.setResponse(LOC("GENFAIL"));
			}
		}
	};
}
