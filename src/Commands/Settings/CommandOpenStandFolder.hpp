#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"

#include <cstdlib>
#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand
{
	class CommandOpenStandFolder : public CommandPhysical
	{
	public:
		explicit CommandOpenStandFolder(CommandList* parent) :
		    CommandPhysical(COMMAND_ACTION, parent, LIT("Open Stand Folder"), CMDNAMES("openstandfolder"), LIT("Opens your %appdata%/StandEnhanced folder."))
		{
		}

		void onClick(Click& click) override
		{
			const auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced";
			ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
		}
	};
}
