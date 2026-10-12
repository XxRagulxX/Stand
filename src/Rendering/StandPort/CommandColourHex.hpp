#pragma once
#include "Rendering/StandPort/CommandAction.hpp"
#include "Commands/Widgets/CommandIssuable.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Menu/Click.hpp"
#include "Util/lang.hpp"

namespace Stand
{
	class CommandColourHex : public CommandAction
	{
	public:
		explicit CommandColourHex(CommandList* const parent)
		    : CommandAction(parent, LOC("CLRHEX"), {}, NOLABEL, CMDFLAGS_ACTION | CMDFLAG_FEATURELIST_SKIP)
		{
		}

		[[nodiscard]] std::wstring getCommandSyntax() const final
		{
			std::wstring syntax{ LANG_GET_W("CMD") };
			syntax.append(L": ").append(cmdNameToUtf16(parent->command_names.at(0))).push_back(L' ');
			syntax.append(LANG_GET_W("ARGVAL"));
			return syntax;
		}

		void onClick(Click& click) final
		{
			std::wstring prefill;
			if (!parent->command_names.empty())
				prefill = cmdNameToUtf16(parent->command_names.at(0));
			prefill.push_back(L' ');
			click.showCommandBoxIfPossible(std::move(prefill));
		}
	};
}
