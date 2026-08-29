#include "Commands/Player/CommandHistoricPlayerWhitelist.hpp"

#include "Commands/Player/CommandHistoricPlayer.hpp"
#include "Network/PlayerHistory.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Localization/lang.hpp"

namespace Stand
{
	CommandHistoricPlayerWhitelist::CommandHistoricPlayerWhitelist(CommandList* parent)
		: CommandToggleNoCorrelation(parent, LOC("WHTJOIN"), {}, LOC("WHTJOIN_H"), false, CMDFLAGS_TOGGLE_NO_CORRELATION | CMDFLAG_TEMPORARY)
	{
	}

	void CommandHistoricPlayerWhitelist::onEnable(Click& click)
	{
		parent->as<CommandHistoricPlayer>()->player->flags |= HP_WHITELIST;
		PlayerHistory::save();
	}

	void CommandHistoricPlayerWhitelist::onDisable(Click& click)
	{
		parent->as<CommandHistoricPlayer>()->player->flags &= ~HP_WHITELIST;
		PlayerHistory::save();
	}

	void CommandHistoricPlayerWhitelist::onTickInGameViewport()
	{
		setOnIndication(parent->as<CommandHistoricPlayer>()->player->flags & HP_WHITELIST);
	}

	void CommandHistoricPlayerWhitelist::onTickInWebViewport()
	{
		return onTickInWebViewportImplRedirect();
	}
}
