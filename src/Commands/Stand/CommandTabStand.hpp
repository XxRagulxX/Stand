#pragma once

#include "Commands/Widgets/CommandTab.hpp"
#include "Commands/Extra/CommandListInfoOverlay.hpp"
#include "Commands/Stand/CommandClearNotifications.hpp"
#include "Commands/Stand/CommandConsole.hpp"
#include "Commands/Stand/CommandEmptyLog.hpp"
#include "Commands/Stand/CommandFeatureList.hpp"
#include "Commands/Stand/CommandOpenStandFolder.hpp"
#include "Commands/Stand/CommandUnload.hpp"

namespace Stand
{
	class CommandTabStand : public CommandTab
	{
	private:
		CommandList* secret = nullptr;

	public:
		CommandConsole* const console;
		CommandOpenStandFolder* const openStandFolder;
		CommandClearNotifications* const clearNotifications;
		CommandEmptyLog* const emptyLog;
		CommandFeatureList* const featureList;
		CommandListInfoOverlay* const infoOverlay;
		CommandUnload* const unload;

		explicit CommandTabStand(CommandList* const parent);

		void populate();

		void onKonamiCodeEntered();
	};
}

namespace Stand::Features
{
	Stand::CommandTabStand& GetCommandTabStand();
}
