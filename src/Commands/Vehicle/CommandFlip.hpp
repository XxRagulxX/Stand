#pragma once

#include "Commands/Extra/CommandActionScript.hpp"

#include "Core/AbstractEntity.hpp"
#include "Game/natives.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandFlip : public CommandActionScript
	{
	public:
		explicit CommandFlip(CommandList* const parent)
			: CommandActionScript(parent, LOC("FLIPVEH"), { CMDNAME("flipvehicle") }, LOC("FLIPVEH_H"))
		{
		}

		void onClickScriptThread(Click& click) final
		{
			Vehicle veh = Util::getVehicle(click);
			if (veh != 0)
			{
				ENTITY::SET_ENTITY_ROTATION(veh, 0, 0, CAM::GET_GAMEPLAY_CAM_ROT(0).z, 0, true);
			}
		}
	};
}
