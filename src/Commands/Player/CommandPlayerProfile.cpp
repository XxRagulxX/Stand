#include "Commands/Player/CommandPlayerProfile.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Rendering/Gui.hpp"
#include "Network/PlayerProvider.hpp"
#include "Network/players_interface.hpp"
#include "Game/pointers.hpp"
#include "Network/rlPc.hpp"

namespace Stand
{
	CommandPlayerProfile::CommandPlayerProfile(CommandList* const parent)
		: CommandActionScript(parent, LOC("PLYPRFL"), CMDNAMES_OBF("profile"), NOLABEL, CMDFLAGS_ACTION_SCRIPT | CMDFLAG_TEMPORARY)
	{
		PC_PTR->registerCommandNames(this);
	}

	void CommandPlayerProfile::onClickScriptThread(Click& click)
	{
		g_gui.prepareForOverlay();
		pointers::g_rlPc->player_manager->ShowPlayerProfileUi(PP_PTR->getPlayers(true)[0].getRockstarId());
	}
}
