#include "Commands/Self/CommandOutfitComponentVariation.hpp"

#include "Commands/Self/CommandLockOutfit.hpp"
#include "Commands/Self/CommandOutfitComponent.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandOutfitComponentVariation::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			const int ped = Self::GetPed().GetHandle();
			if (data->id == PV_COMP_HAIR)
			{
				const int drawable = CommandLockOutfit::ped_component_commands[PV_COMP_HAIR].first->value;
				PED::SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HAIR, drawable, 0, 0);
				int highlight = CommandLockOutfit::hair_highlight->value;
				if (highlight == -1)
					highlight = value;
				PED::SET_PED_HAIR_TINT(ped, value, highlight);
			}
			else
			{
				const int drawable = CommandLockOutfit::ped_component_commands[data->id].first->value;
				PED::SET_PED_COMPONENT_VARIATION(ped, data->id, drawable, value, PED::GET_PED_PALETTE_VARIATION(ped, data->id));
			}
		});
	}

	void CommandOutfitComponentVariation::onDrawableChange()
	{
		Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
		setValueIndicator(click, 0);
		updateRange();
	}

	void CommandOutfitComponentVariation::updateRange()
	{
		const int ped = Self::GetPed().GetHandle();
		const int drawable = CommandLockOutfit::ped_component_commands[data->id].first->value;
		setMaxValue(PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(ped, data->id, drawable) - 1);
	}
}
