#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Rendering/Gui.hpp"
#include "Network/PlayerProvider.hpp"

namespace Stand
{
	class CommandPlayerAs : public CommandAction
	{
	public:
		explicit CommandPlayerAs(CommandList* const parent)
			: CommandAction(parent, LOC("CMDBHLF"), CMDNAMES_OBF("as"), NOLABEL, CMDFLAGS_ACTION | CMDFLAG_TEMPORARY, COMMANDPERM_NEUTRAL)
		{
			PC_PTR->registerCommandNames(this);
		}

		void onClick(Click& click) final
		{
			std::wstring prefill{ L"as " };
			prefill.append(cmdNameToUtf16(command_names.at(0).substr(2))).push_back(L' ');
			click.showCommandBoxIfPossible(std::move(prefill));
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			click.issued_for = PP_PTR->getPlayers(true).at(0);
			click.issuer_is_explicit = true;
		}
	};
}
