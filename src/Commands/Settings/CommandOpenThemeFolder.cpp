#include "Commands/Widgets/CommandLambdaAction.hpp"

#include <cstdlib>
#include <filesystem>
#include <windows.h>
#include <shellapi.h>

namespace Stand::Features
{
	static StandWidgets::CommandLambdaAction _OpenThemeFolder{"openthemefolder",
	    "Open Theme Folder",
	    "Opens your %appdata%/StandEnhanced folder.",
	    [] {
		    const auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced";
		    ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	    }};
}
