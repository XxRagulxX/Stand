#include "Commands/Player/CommandPlayerCopyOutfit.hpp"

#include "Core/AbstractEntity.hpp"
#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Localization/lang.hpp"
#include "Network/PlayerProvider.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	CommandPlayerCopyOutfit::CommandPlayerCopyOutfit(CommandList* const parent)
		: CommandOnPlayer(parent, LOC("CPOTFT"), CMDNAMES_OBF("copyoutfit"), NOLABEL, CMDFLAGS_ACTION_SCRIPT | CMDFLAG_TEMPORARY)
	{
		PC_PTR->registerCommand(this);
	}

	void CommandPlayerCopyOutfit::onClickScriptThread(Click& click)
	{
		const AbstractPlayer p = PP_PTR->getPlayer();
		if (p == g_player)
		{
			click.uwotm8();
		}
		else
		{
			auto ped = p.getPed();
			Util::loadOutfit(this, click, Util::get_colons_outfit_from_ped(ped));
		}
	}
}
