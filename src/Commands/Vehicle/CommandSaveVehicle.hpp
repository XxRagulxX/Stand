#pragma once

#include "Commands/Widgets/CommandActionName.hpp"

#include "Vehicle/ColonsVehicleCustomisationIo.hpp"
#include "Util/Util.hpp"
#include "Vehicle/VehicleEntityCustomisationIo.hpp"

namespace Stand
{
	class CommandSaveVehicle : public CommandActionName
	{
	public:
		explicit CommandSaveVehicle(CommandList* parent)
			: CommandActionName(parent, LOC("SVEVEH"), CMDNAMES("savevehicle"))
		{
		}

		void onClick(Click& click) final
		{
			if (Util::getVehicle(click).isValid())
			{
				CommandAction::onClick(click);
			}
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			if (!onCommandValidateFileName(click, args))
			{
				return;
			}
			auto veh = Util::getVehicle(click);
			if (!veh.isValid())
			{
				args.clear();
				return;
			}
			VehicleEntityCustomisationIo veh_cio{ veh };
			SOUP_IF_LIKELY (ColonsVehicleCustomisationIo::handleSaveCommand(click, args, veh_cio, &veh))
			{
				((CommandListFolder*)parent)->refresh();
			}
		}
	};
}
