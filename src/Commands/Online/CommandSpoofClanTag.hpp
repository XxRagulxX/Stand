#pragma once

#include "Commands/Widgets/CommandInput.hpp"

namespace Stand
{
	class CommandSpoofClanTag : public CommandInput
	{
	public:
		explicit CommandSpoofClanTag(CommandList* const parent)
			: CommandInput(parent, LOC("CRWTG"), { CMDNAME("crewtag") }, NOLABEL, "STD")
		{
		}

		std::wstring getCommandSyntax() const final
		{
			std::wstring syntax{ LANG_GET_W("CMD") };
			syntax.append(L": ").append(cmdNameToUtf16(command_names.at(0))).append(L" [");
			syntax.append(StringUtils::to_lower(LANG_GET_W("CRWTG"))).push_back(L']');
			return syntax;
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			auto arg = get_next_arg(args);
			if (checkArgsLength(click, arg, 4))
			{
				const auto tag = StringUtils::utf16_to_utf8(arg);
				strcpy_s(g_hooking.spoofed_clan_membership.clan.tag, sizeof(g_hooking.spoofed_clan_membership.clan.tag), tag.c_str());
				setValueFromHooking(click.type);
				if (g_hooking.spoof_clan_membership)
				{
					parent->as<CommandSpoofClan>()->updateData();
				}
			}
		}

		void getExtraInfo(CommandExtraInfo& info, std::wstring& args) final
		{
			CommandInput::getExtraInfo(info, args);
			info.char_limit = 4;
		}

		void setValueFromHooking(ClickType click_type)
		{
			setValue(g_hooking.spoofed_clan_membership.clan.tag, click_type);
		}
	};
}
