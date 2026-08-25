#pragma once

#include "Commands/Extra/CommandActionScript.hpp"

#include "Core/AbstractEntity.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandVehicleDestroy : public CommandActionScript
	{
	public:
		explicit CommandVehicleDestroy(CommandList* const parent)
			: CommandActionScript(parent, LOC("DSTRY"), { CMDNAME("destroyvehicle") })
		{
		}

		void onClickScriptThread(Click& click) final
		{
			auto veh = Util::getVehicle(click);
			if (veh.isValid())
			{
				veh.destroy();
			}
		}
	};
}
