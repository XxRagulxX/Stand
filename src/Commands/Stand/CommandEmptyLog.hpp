#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>

namespace Stand
{
	class CommandEmptyLog : public CommandPhysical
	{
	public:
		explicit CommandEmptyLog(CommandList* parent) :
		    CommandPhysical(COMMAND_ACTION, parent, LIT("Empty Log.txt"), CMDNAMES("emptylog"), LIT("Cleanses your %appdata%/StandEnhanced/cout.log."))
		{
		}

		void onClick(Click& click) override
		{
			const auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced" / "cout.log";
			std::ofstream(path, std::ios::out | std::ios::trunc).close();
		}
	};
}
