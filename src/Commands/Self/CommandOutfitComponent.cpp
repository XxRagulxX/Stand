#include "Commands/Self/CommandOutfitComponent.hpp"

#include "Commands/Self/CommandLockOutfit.hpp"
#include "Commands/Self/CommandOutfitComponentVariation.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandOutfitComponent::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			const int ped = Self::GetPed().GetHandle();
			if (data->id == PV_COMP_HAIR)
			{
				PED::SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HAIR, value, 0, 0);
				CommandLockOutfit::setHairColourFromCommands();
			}
			else
			{
				PED::SET_PED_COMPONENT_VARIATION(ped, data->id, value, 0, PED::GET_PED_PALETTE_VARIATION(ped, data->id));
				CommandLockOutfit::ped_component_commands[data->id].second->onDrawableChange();
			}
		});
	}
}
