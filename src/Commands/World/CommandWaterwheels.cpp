#include "Commands/World/CommandWaterwheels.hpp"

#include "Core/AbstractEntity.hpp"
#include "Game/gta_vehicle.hpp"
#include "Game/phArchetype.hpp"
#include "Util/Util.hpp"
#include "Vehicle/VehicleType.hpp"

namespace Stand
{
	CommandWaterwheels::CommandWaterwheels(CommandList* const parent)
		: CommandToggle(parent, LOC("DRVUNDWTR"), { CMDNAME("waterwheels"), CMDNAME("driveunderwater"), CMDNAME("waterdrive") })
	{
	}

	void CommandWaterwheels::onChange(Click& click)
	{
		onChangeToggleScriptTickEventHandler(click, []
		{
			if (auto e = Util::getVehicle(); e.isValid())
			{
				if (auto v = (CVehicle*)e.getPointer())
				{
					if (v->vehicle_type != VEHICLE_TYPE_BOAT && v->vehicle_type != VEHICLE_TYPE_SUBMARINE)
					{
						if (v->ph_inst && v->ph_inst && v->ph_inst->archetype)
						{
							reinterpret_cast<rage::phArchetypePhys*>(v->ph_inst->archetype)->m_BuoyancyFactor = 0.0f;
						}
					}
				}
			}
		});
	}
}
