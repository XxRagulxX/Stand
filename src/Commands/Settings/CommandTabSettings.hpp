#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Settings/CommandClearNotifications.hpp"
#include "Commands/Settings/CommandConsole.hpp"
#include "Commands/Settings/CommandEmptyLog.hpp"
#include "Commands/Settings/CommandFeatureList.hpp"
#include "Commands/Settings/CommandOpenStandFolder.hpp"
#include "Commands/Settings/CommandUnload.hpp"

namespace Stand
{
	class CommandTabSettings : public CommandList
	{
	public:
		CommandConsole* const console;
		CommandOpenStandFolder* const openStandFolder;
		CommandClearNotifications* const clearNotifications;
		CommandEmptyLog* const emptyLog;
		CommandFeatureList* const featureList;
		CommandUnload* const unload;

		explicit CommandTabSettings() :
		    CommandList(nullptr, LIT("Settings")),
		    console(createChild<CommandConsole>()),
		    openStandFolder(createChild<CommandOpenStandFolder>()),
		    clearNotifications(createChild<CommandClearNotifications>()),
		    emptyLog(createChild<CommandEmptyLog>()),
		    featureList(createChild<CommandFeatureList>()),
		    unload(createChild<CommandUnload>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabSettings& GetCommandTabSettings();
}
