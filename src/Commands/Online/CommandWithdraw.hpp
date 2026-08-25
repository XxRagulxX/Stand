#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Util/get_next_arg.hpp"
#include "Game/natives.hpp"
#include "Util/str2int.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandWithdraw : public CommandAction
	{
	public:
		explicit CommandWithdraw(CommandList* parent)
			: CommandAction(parent, LOC("WTHDRW"), { CMDNAME("withdraw") })
		{
		}

		std::wstring getCommandSyntax() const final
		{
			return std::move(LANG_GET_W("CMD").append(L": withdraw ").append(LANG_GET_W("ARGVAL")));
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			auto arg = get_next_arg(args);
			if (arg.empty())
			{
				return onClick(click);
			}
			auto res = str2int<int>(arg);
			if (!res.has_value())
			{
				click.setResponse(LOC("INVARG"));
				click.showCommandBoxIfPossible(std::move(std::wstring(L"withdraw ").append(arg)));
				return;
			}
			int val = res.value();
			const int char_slot = Util::getCharSlot(click);
			if (char_slot != -1)
			{
				const int bank = MONEY::NETWORK_GET_VC_BANK_BALANCE();
				if (bank < val)
				{
					val = bank;
				}
				if (val != 0)
				{
				}
				NETSHOPPING::NET_GAMESERVER_TRANSFER_BANK_TO_WALLET(char_slot, val);
			}
		}
	};
}
