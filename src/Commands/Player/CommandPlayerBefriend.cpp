#include "Commands/Player/CommandPlayerBefriend.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Rendering/Gui.hpp"
#include "Network/PlayerProvider.hpp"
#include "Network/players_interface.hpp"
#include "Game/pointers.hpp"
#include "Network/rlPc.hpp"

namespace Stand
{
	CommandPlayerBefriend::CommandPlayerBefriend(CommandList* const parent)
		: CommandActionScript(parent, LOC("PLYFRND"), CMDNAMES_OBF("befriend"), NOLABEL, CMDFLAGS_ACTION_SCRIPT | CMDFLAG_TEMPORARY)
	{
		PC_PTR->registerCommandNames(this);
	}

	void CommandPlayerBefriend::onClickScriptThread(Click& click)
	{
		g_gui.prepareForOverlay();
		pointers::g_rlPc->player_manager->ShowFriendRequestUi(PP_PTR->getPlayers(true)[0].getRockstarId());
	}
}
