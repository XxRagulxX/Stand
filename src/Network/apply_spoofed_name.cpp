#include "Network/apply_spoofed_name.hpp"

#include "Core/AbstractEntity.hpp"
#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandPlayer.hpp"
#include "Game/CPlayerInfo.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Game/pointers.hpp"
#include "Network/rlGamerInfo.hpp"

namespace Stand
{
	void apply_spoofed_name(const char* name)
	{
		g_player.setOverrideName(name);
		strcpy(pointers::rlPresence_m_ActingGamerInfo->name, name);
	}

	void on_name_spoofing_update()
	{
		if (auto cmd = g_player.getCommand())
		{
			cmd->force_recreate = true;
		}
	}
}
