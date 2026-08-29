#pragma once

#include "Commands/Widgets/CommandSliderScriptTick.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"

#include "Game/natives.hpp"

namespace Stand
{
	class CommandWaterStrengthOverride : public CommandSliderScriptTick<CommandSliderFloat>
	{
	public:
		explicit CommandWaterStrengthOverride(CommandList* parent)
			: CommandSliderScriptTick(parent, LOC("STROVR"), { CMDNAME("waterstrength") }, NOLABEL, 0, 100000, 0, 10)
		{
		}

		Label getActivationName() const final
		{
			return getActivationNameImplCombineWithParent();
		}

		void onScriptTick() final
		{
			MISC::WATER_OVERRIDE_SET_STRENGTH(getFloatValue());
		}
	};
}
