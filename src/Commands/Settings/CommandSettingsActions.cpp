#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Commands/Widgets/CommandLambdaToggle.hpp"
#include "Commands/CommandLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Core/FileMgr.hpp"
#include "Core/LogHelper.hpp"
#include "Rendering/Notifications.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <windows.h>
#include <shellapi.h>

namespace Stand::Features
{
	static StandWidgets::CommandLambdaToggle _Console{"console",
	    "Console",
	    "",
	    [] { LogHelper::ToggleConsole(true); },
	    [] { LogHelper::ToggleConsole(false); }};

	static StandWidgets::CommandLambdaAction _OpenStandFolder{"openstandfolder",
	    "Open Stand Folder",
	    "Opens your %appdata%/StandEnhanced folder.",
	    [] {
		    const auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced";
		    ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	    }};

	static StandWidgets::CommandLambdaAction _ClearNotifys{"clearstandnotifys",
	    "Clear Notifications",
	    "Clears Stand's notification system.",
	    [] {
		    Notifications::Clear();
	    }};

	static StandWidgets::CommandLambdaAction _EmptyLog{"emptylog",
	    "Empty Log.txt",
	    "Cleanses your %appdata%/StandEnhanced/cout.log.",
	    [] {
		    const auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced" / "cout.log";
		    std::ofstream(path, std::ios::out | std::ios::trunc).close();
	    }};

	static StandWidgets::CommandLambdaAction _FeatureList{"featurelist",
	    "Generate Features (en).txt",
	    "Saves all currently available commands in %appdata%/StandEnhanced/Features (en).txt with their help text and syntax.",
	    [] {
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
	    }};
}
