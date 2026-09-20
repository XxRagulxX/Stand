#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Config/ProfileManager.hpp"
#include "Menu/Click.hpp"

#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand
{
	class CommandOpenProfilesFolder : public CommandPhysical
	{
	public:
		explicit CommandOpenProfilesFolder(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Open Profiles Folder"), CMDNAMES("openprofilesfolder"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			const auto path = ProfileManager::GetProfilesPath();
			std::filesystem::create_directories(path);
			ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
		}
	};

	class CommandTabProfiles : public CommandList
	{
	public:
		CommandOpenProfilesFolder* const openProfilesFolder;

		explicit CommandTabProfiles()
			: CommandList(nullptr, LIT("Profiles"), CMDNAMES())
			, openProfilesFolder(createChild<CommandOpenProfilesFolder>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabProfiles& GetCommandTabProfiles();
}
