#include "Commands/Online/CommandListSpoofing.hpp"

#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "AntiCheat/ComponentSpoofPos.hpp"

#include "Commands/Online/CommandSpoofName.hpp"
#include "Commands/Online/CommandSpoofedName.hpp"
#include "Commands/Online/CommandSpoofedNameFromHistory.hpp"

#if false
#include "Commands/Online/CommandSpoofPresets.hpp"
#endif

#if false
#include "Commands/Online/CommandSpoofHostToken.hpp"
#include "Commands/Online/CommandSpoofedHostToken.hpp"
#include "Commands/Online/CommandHostTokenPresets.hpp"
#include "Commands/Online/CommandAutoKickHost.hpp"
#endif

#include "Commands/Online/CommandListSessionSpoof.hpp"
#include "Commands/Online/CommandSpoofClan.hpp"
#include "Commands/Online/CommandSpoofRank.hpp"
#include "Commands/Online/CommandSpoofKd.hpp"

#include "Commands/Online/CommandSpoofedPosition.hpp"

#include "Commands/Extra/CommandDevFlag.hpp"
#include "Commands/Online/CommandStreamerSpoof.hpp"
#include "Commands/Player/CommandHideFromPlayerList.hpp"
#include "Commands/Online/CommandPhoneModeSpoof.hpp"

namespace Stand
{
	CommandListSpoofing::CommandListSpoofing(CommandList* const parent)
		: CommandList(parent, LOC("SPF"))
	{
		auto name_spoofing = this->createChild<CommandList>(LOC("SPFNME_D"), {}, LOC("GEN_H_LOC"));
		name_spoofing->createChild<CommandSpoofName>();
		name_spoofing->createChild<CommandSpoofedName>();
		name_spoofing->createChild<CommandSpoofedNameFromHistory>();

#if false
		this->createChild<CommandSpoofPresets>();
#endif

#if false
		{
			auto host_token_spoofing = this->createChild<CommandList>(LOC("SPFHT_D"), {}, LOC("SPFHT_H"));
			auto spoof_host_token = host_token_spoofing->createChild<CommandSpoofHostToken>();
			auto spoofed_host_token = host_token_spoofing->createChild<CommandSpoofedHostToken>(spoof_host_token);
			host_token_spoofing->createChild<CommandHostTokenPresets>(spoofed_host_token);
			{
				auto kick_host_list = host_token_spoofing->createChild<CommandList>(LOC("AKCKHST"));
				auto kick_host = kick_host_list->createChild<CommandAutoKickHost>();
				kick_host->excludes.populateList(kick_host_list, {}, PlayerExcludes::SHOW_MODDER | PlayerExcludes::SHOW_ORG_MEMBERS | PlayerExcludes::SHOW_LIKELY_MODDER);
			}
		}
#endif

		this->createChild<CommandListSessionSpoof>();
		this->createChild<CommandSpoofClan>();
		this->createChild<CommandSpoofRank>();
		this->createChild<CommandSpoofKd>();
		{
			auto pos_spoofing = this->createChild<CommandList>(LOC("SPFPOS"));

			pos_spoofing->createChild<CommandTogglePointer>(&g_comp_spoofpos.spoof_position, LOC("SPFPOS"), CMDNAMES_OBF("spoofpos"));
			pos_spoofing->createChild<CommandSpoofedPosition>();
		}
		this->createChild<CommandDevFlag>();
		this->createChild<CommandTogglePointer>(&g_hooking.spoof_qa_flag, LOC("MDDRDT_RSQA"), { CMDNAME("qaflag") }, LOC("SPF_H_FLAG"));
		this->createChild<CommandTogglePointer>(&g_hooking.spoof_cheater_flag, LOC("MDDRDT_CHEAT"), { CMDNAME("cheaterflag") }, LOC("SPF_H_FLAG"));
		if (g_gui.root_state != GUI_MINIMAL)
		{
			this->createChild<CommandStreamerSpoof>();
			this->createChild<CommandHideFromPlayerList>();
		}
		this->createChild<CommandPhoneModeSpoof>();
	}
}
