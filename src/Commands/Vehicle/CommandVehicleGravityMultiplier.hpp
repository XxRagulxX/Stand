#pragma once

#include "Commands/Widgets/CommandSliderFloat.hpp"

#include "Game/natives.hpp"
#include "Game/pointers.hpp"

namespace Stand
{
	class CommandVehicleGravityMultiplier : public CommandSliderFloat
	{
	public:
		CommandToggle* not_in_aircraft;

		explicit CommandVehicleGravityMultiplier(CommandList* const parent)
			: CommandSliderFloat(parent, LOC("GRVTYMTPLY"), { CMDNAME("gravitymultiplier") }, NOLABEL, -10000, 10000, 100, 10)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			const int value = this->value;
			const float fvalue = ((float)this->value / 100.0f) * 9.8000002f;
			if (value != 100)
			{
				registerScriptTickEventHandler(click, [=, this]()
				{
					if (value != this->value)
					{
						return false;
					}
					auto& veh = g_player_veh;
					if (veh.isValid() && veh.isOwnerOfVehicleAndDriver())
					{
						if (not_in_aircraft->m_on && veh.getModel().isAirborne())
						{
							pointers::CVehicle_SetGravityForWheellIntegrator(veh, 9.8000002f);
						}
						else
						{
							pointers::CVehicle_SetGravityForWheellIntegrator(veh, fvalue);
						}
					}
					return true;
				});
			}
			else
			{
				ensureScriptThread(click, [=]
				{
					auto veh = g_player_ped.getVehicle(true);
					if (veh.isValid() && veh.isOwnerOfVehicleAndDriver())
					{
						pointers::CVehicle_SetGravityForWheellIntegrator(veh, fvalue);
					}
				});
			}
		}
	};
}
