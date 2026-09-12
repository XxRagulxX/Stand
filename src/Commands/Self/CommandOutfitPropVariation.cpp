#include "Commands/Self/CommandOutfitPropVariation.hpp"

#include "Commands/Self/CommandLockOutfit.hpp"
#include "Commands/Self/CommandOutfitProp.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandOutfitPropVariation::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			const int ped = Self::GetPed().GetHandle();
			const int drawable = CommandLockOutfit::ped_prop_commands[data->id].first->value;
			PED::SET_PED_PROP_INDEX(ped, data->id, drawable, value, TRUE, 1);
		});
	}

	void CommandOutfitPropVariation::onDrawableChange()
	{
		Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
		setValueIndicator(click, 0);
		updateRange();
	}

	void CommandOutfitPropVariation::updateRange()
	{
		const int ped = Self::GetPed().GetHandle();
		const int drawable = CommandLockOutfit::ped_prop_commands[data->id].first->value;
		setMaxValue(PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(ped, data->id, drawable) - 1);
	}
}
