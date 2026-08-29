#pragma once

#include "Commands/Widgets/CommandSearch.hpp"

#include "Commands/Widgets/CommandLink.hpp"
#include "Commands/Vehicle/CommandVehicle.hpp"
#include "Vehicle/get_vehicle_display_name.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandFindVehicle : public CommandSearch
	{
	private:
		CommandToggleNoCorrelation* const manufacturer_names;

	public:
		explicit CommandFindVehicle(CommandList* const parent, CommandToggleNoCorrelation* manufacturer_names)
			: CommandSearch(parent, LOC("SRCH"), { CMDNAME("findvehicle") }), manufacturer_names(manufacturer_names)
		{
		}

		void doSearch(std::string&& arg) final
		{
			StringUtils::simplifyCommandName(arg);
			auto* const dlcs = (CommandList*)parent->resolveChildByMenuName(LOC("DLCS"));
			for (auto& dlc : dlcs->children)
			{
				if (!dlc->isList())
				{
					continue;
				}
				for (auto& _vehicle : ((CommandList*)dlc.get())->children)
				{
					auto* vehicle = ((CommandVehicle*)_vehicle.get());
					std::string search_name = get_vehicle_display_name(vehicle->data);
					StringUtils::simplifyCommandName(search_name);
					if (search_name.find(arg) != std::string::npos)
					{
						createChild<CommandLink>(vehicle);
					}
				}
			}
		}
	};
}
