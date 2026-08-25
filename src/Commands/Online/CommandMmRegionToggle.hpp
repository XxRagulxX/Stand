#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Network/rlRosGeoLocInfo.hpp"
#include "Game/pointers.hpp"

namespace Stand
{
	class CommandMmRegionToggle : public CommandToggle
	{
	public:
		explicit CommandMmRegionToggle(CommandList* const parent)
			: CommandToggle(parent, LOC("MMREGION2"), CMDNAMES_OBF("regionoverride", "overrideregion"))
		{
		}

		void onEnable(Click& click) final
		{
			parent->as<CommandMmRegion>()->og_region = pointers::rlRos_s_GeoLocInfo->region_code;
			pointers::rlRos_s_GeoLocInfo->region_code = (uint32_t)parent->as<CommandMmRegion>()->select->value;
		}

		void onDisable(Click& click) final
		{
			pointers::rlRos_s_GeoLocInfo->region_code = (uint32_t)parent->as<CommandMmRegion>()->og_region;
		}
	};
}
