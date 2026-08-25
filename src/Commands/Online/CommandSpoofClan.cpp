#include "Commands/Online/CommandSpoofClan.hpp"

#include <cstring>

#include "Commands/Stand/CommandExtraInfo.hpp"
#include "Core/ExecCtx.hpp"
#include "Util/get_next_arg.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Network/is_session.hpp"
#include "Localization/lang.hpp"
#include "Game/natives.hpp"
#include "Game/pointers.hpp"
#include "Network/rlClanClient.hpp"
#include "Network/scrClanDesc.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Util/StringUtils.hpp"

#include "Commands/Online/CommandSpoofClanToggle.hpp"

#include "Commands/Online/CommandSpoofClanId.hpp"
#include "Commands/Online/CommandSpoofClanTag.hpp"
#include "Commands/Online/CommandSpoofClanName.hpp"
#include "Commands/Online/CommandSpoofClanMotto.hpp"
#include "Commands/Online/CommandSpoofClanAlt.hpp"
#include "Commands/Online/CommandSpoofClanColour.hpp"

#include "Commands/Online/CommandSpoofClanPreset.hpp"
#include "Commands/Online/CommandSpoofClanMine.hpp"

namespace Stand
{
	CommandSpoofClan::CommandSpoofClan(CommandList* const parent)
		: CommandList(parent, LOC("SPFCRW"))
	{
		instance = this;

		this->createChild<CommandSpoofClanToggle>();
		id = this->createChild<CommandSpoofClanId>();
		tag = this->createChild<CommandSpoofClanTag>();
		name = this->createChild<CommandSpoofClanName>();
		motto = this->createChild<CommandSpoofClanMotto>();
		alt = this->createChild<CommandSpoofClanAlt>();
		colour = this->createChild<CommandSpoofClanColour>();

		{
			auto presets = this->createChild<CommandList>(LOC("PRSTS"));

			presets->createChild<CommandSpoofClanPreset>(0, LOC("NCRW"), "", "", "");
			presets->createChild<CommandSpoofClanPreset>(133742069, "STD", soup::ObfusString("Stand.sh"), "", true);
			presets->createChild<CommandSpoofClanPreset>(42604020, "DEAD", "Runs Dead", "Don't do the time, sub with Twitch Prime");
			presets->createChild<CommandSpoofClanPreset>(735, "IGN", "IGN Crew", "Obsessed with Gaming, Entertainment, and Everything guys enjoy.", true);
		}

		this->createChild<CommandSpoofClanMine>();
	}

	Label CommandSpoofClan::getClanLabel(const char* const tag, const std::string& name, const bool alt_badge)
	{
		std::string str = name;
		str.append(alt_badge ? " < " : " [ ");
		str.append(tag);
		str.append(" ]");
		return LIT(std::move(str));
	}

	void CommandSpoofClan::setValuesFromHooking(Click& click) const
	{
		id->setValueFromHooking(click.type);
		tag->setValueFromHooking(click.type);
		name->setValueFromHooking(click.type);
		motto->setValueFromHooking(click.type);
		alt->setValueFromHooking(click);
		colour->setValueFromHooking(click);

		updateData();
	}

	void CommandSpoofClan::updateData()
	{
		if (is_session_started())
		{
			g_hooking.forceUpdateGamerDataNode();

			ExecCtx::get().ensureScript([]
				{
					scrClanDesc& desc = ScriptGlobal(GLOBAL_CLAN_DESCS).at(g_player, 35).as<scrClanDesc&>();
					rage::rlClanMembershipData& data = *pointers::rage_rlClan_GetPrimaryMembership(0);

					desc.id.Int = static_cast<int>(data.clan.id);
					desc.r.Int = data.clan.r;
					desc.g.Int = data.clan.g;
					desc.b.Int = data.clan.b;

					strcpy_s(desc.tag, sizeof(desc.tag), data.clan.tag);
					strcpy_s(desc.name, sizeof(desc.name), data.clan.name);

					desc.alt_badge.Int = data.clan.alt_badge ? 1 : 0;
				});
		}
	}
}
