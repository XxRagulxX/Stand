#include "Commands/Self/CommandOutfitComponent.hpp"

#include "Core/AbstractEntity.hpp"
#include "Commands/Self/CommandOutfitComponentVariation.hpp"
#include "Commands/Self/CommandLockOutfit.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	void CommandOutfitComponent::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			if (data->id == PV_COMP_HAIR)
			{
				PED::SET_PED_COMPONENT_VARIATION(g_player_ped, PV_COMP_HAIR, value, 0, 0);
				CommandLockOutfit::setHairColourFromCommands();
			}
			else
			{
				PED::SET_PED_COMPONENT_VARIATION(g_player_ped, data->id, value, 0, 0);
				CommandLockOutfit::ped_component_commands[data->id].second->onDrawableChange();
			}
		});
	}
}
