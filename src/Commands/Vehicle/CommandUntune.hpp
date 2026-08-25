#pragma once

#include "Commands/Extra/CommandActionScript.hpp"

#include "Game/atStringHash.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	class CommandUntune : public CommandActionScript
	{
	public:
		explicit CommandUntune(CommandList* const parent)
			: CommandActionScript(parent, LOC("DNGRD"), { CMDNAME("untune"), CMDNAME("downgrade") }, LOC("DNGRD_H"))
		{
		}

		void onClickScriptThread(Click& click) final
		{
			const Vehicle veh = Util::getVehicle(click);
			if (veh)
			{
				AbstractEntity::get(veh).requestControl(ATSTRINGHASH("untune"), [](AbstractEntity& veh)
				{
					veh.downgrade();
				}, [](AbstractEntity&)
				{
					Util::toast(LOC("VEHCTRL"));
				});
			}
		}
	};
}
