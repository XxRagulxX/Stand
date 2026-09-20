#pragma once
#include "Commands/CommandLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

namespace Stand
{
	class CommandFeatureList : public CommandPhysical
	{
	public:
		explicit CommandFeatureList(CommandList* parent) :
		    CommandPhysical(COMMAND_ACTION, parent, LIT("Generate Features (en).txt"), CMDNAMES("featurelist"), LIT("Saves all currently available commands in %appdata%/StandEnhanced/Features (en).txt with their help text and syntax."))
		{
		}

		void onClick(Click& click) override
		{
			const auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced" / "Features (en).txt";

			std::map<std::string, CommandLegacy*> sorted;
			for (auto& [hash, cmd] : Commands::GetCommands())
				if (!cmd->GetLabel().empty())
					sorted.emplace(cmd->GetLabel(), cmd);

			std::ofstream file(path, std::ios::out | std::ios::trunc);
			for (auto& [label, cmd] : sorted)
			{
				file << label << "\n";
				file << "  Command: " << cmd->GetName() << "\n";
				if (!cmd->GetDescription().empty())
					file << "  " << cmd->GetDescription() << "\n";
				file << "\n";
			}
		}
	};
}
