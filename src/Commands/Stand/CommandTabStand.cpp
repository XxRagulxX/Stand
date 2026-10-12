#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand
{
	CommandTabStand::CommandTabStand(CommandList* const parent)
		: CommandTab(parent, TAB_STAND, LIT("Stand"), { CMDNAME("tstand"), CMDNAME("stand") }),
		  console(createChild<CommandConsole>()),
		  openStandFolder(createChild<CommandOpenStandFolder>()),
		  clearNotifications(createChild<CommandClearNotifications>()),
		  emptyLog(createChild<CommandEmptyLog>()),
		  featureList(createChild<CommandFeatureList>()),
		  infoOverlay(createChild<CommandListInfoOverlay>()),
		  unload(createChild<CommandUnload>())
	{
	}

	void CommandTabStand::populate()
	{
	}

	void CommandTabStand::onKonamiCodeEntered()
	{
	}
}

namespace Stand::Features
{
	Stand::CommandTabStand& GetCommandTabStand()
	{
		static Stand::CommandTabStand instance{ nullptr };
		return instance;
	}
}
