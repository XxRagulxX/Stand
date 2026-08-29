#pragma once

#include "Network/HistoricPlayer.hpp"
#include "Commands/Widgets/CommandAction.hpp"
#include "AntiCheat/Hooking.hpp"

namespace Stand
{
	class CommandUndoRemoved : public CommandAction
	{
	public:
		explicit CommandUndoRemoved(CommandList* const parent, const std::string& name)
			: CommandAction(parent, LIT(name), {}, LOC("BJ_KICKEDP_H"))
		{
		}

		void onClick(Click& click) final
		{
			for (auto i = g_hooking.removed_players.begin(); i != g_hooking.removed_players.end(); ++i)
			{
				if (i->first == menu_name.getLiteralUtf8())
				{
					g_hooking.removed_players.erase(i);
					break;
				}
			}
			this->beGone();
		}
	};
}
