#pragma once

#include "Commands/Widgets/CommandInput.hpp"

#include "Network/apply_spoofed_name.hpp"
#include "Commands/Stand/CommandExtraInfo.hpp"
#include "Util/get_next_arg.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Game/pointers.hpp"
#include "Network/rlGamerInfo.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandSpoofedName : public CommandInput
	{
	public:
		inline static CommandSpoofedName* instance = nullptr;

		explicit CommandSpoofedName(CommandList* const parent)
			: CommandInput(parent, LOC("SPFDNME"), CMDNAMES("spoofedname"), NOLABEL, pointers::rlPresence_GamerPresences_0_GamerInfo->name)
		{
			instance = this;
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			if (checkArgsLength(click, args, 16))
			{
				setName(click, StringUtils::utf16_to_utf8(args));
			}
			args.clear();
		}

		void setName(Click& click, std::string name)
		{
			if (!g_hooking.isSpoofedNameAllowed(name))
			{
				name = "Sussy_Imposter";
			}
			setValueGta(name, click.type);
			g_hooking.spoofed_name = name;
			if (g_hooking.spoof_name)
			{
				apply_spoofed_name(g_hooking.spoofed_name.c_str());
				on_name_spoofing_update();
				//click.setSpoofUpdate();
			}
		}

		void getExtraInfo(CommandExtraInfo& info, std::wstring& args) final
		{
			CommandInput::getExtraInfo(info, args);
			info.char_limit = 16;
			info.colour_selector = true;
		}
	};
}
