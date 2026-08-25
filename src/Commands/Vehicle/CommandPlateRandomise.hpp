#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Game/natives.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlateRandomise : public CommandAction
	{
	public:
		explicit CommandPlateRandomise(CommandList* const parent)
			: CommandAction(parent, LOC("RNDISE"), CMDNAMES_OBF("platerandomise"))
		{
		}

		void onClick(Click& click) final
		{
			if (auto veh = Util::getVehicle(click); veh.isValid())
			{
				veh.randomiseSeedAndUpdatePlate();
			}
		}
	};
}
