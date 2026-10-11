#pragma once

#include "Commands/Widgets/CommandInput.hpp"

#include "Util/lang.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class ScAccount;
	class CommandGamer;

	class CommandInputGamerName : public CommandInput
	{
	private:
		CommandGamer* const gamer;
		ScAccount* scaccount = nullptr;
		bool needs_completion = true;

	public:
		explicit CommandInputGamerName(CommandList* parent, std::string&& menu_name, const std::vector<CommandName>& command_names, CommandGamer* const gamer)
			: CommandInput(parent, LIT(std::move(menu_name.append(" - ").append(LANG_GET("NME")))), combineCommandNames(command_names, CMDNAME("name")), NOLABEL, {}, CMDFLAGS_ACTION_VALUE & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS), gamer(gamer)
		{
		}

		void reset() noexcept
		{
			value.clear();
			scaccount = nullptr;
			needs_completion = true;
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			if (args.empty())
			{
				return onClick(click);
			}
			value = StringUtils::utf16_to_utf8(args);
			args.clear();
			update_state(click.type);
		}

		void onTickInGameViewport() final
		{
		}

		void onTickInWebViewport() final
		{
		}
	};
}
