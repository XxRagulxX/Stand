#pragma once
#include "Rendering/StandPort/CommandAction.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandColourHex : public CommandAction
	{
	public:
		explicit CommandColourHex(CommandList* const parent)
		    : CommandAction(parent, LOC("CLRHEX"), {}, NOLABEL, CMDFLAGS_ACTION | CMDFLAG_FEATURELIST_SKIP)
		{
		}

		[[nodiscard]] std::string getCommandSyntax() const final
		{
			std::string syntax = "Command: ";
			if (!parent->command_names.empty())
				syntax += cmdNameToUtf8(parent->command_names.at(0));
			syntax += " <value>";
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
