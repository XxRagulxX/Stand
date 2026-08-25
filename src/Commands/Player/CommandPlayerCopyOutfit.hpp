#pragma once

#include "Commands/Player/CommandOnPlayer.hpp"
#include "Commands/Extra/CommandActionScript.hpp"

namespace Stand
{
	class CommandPlayerCopyOutfit : public CommandOnPlayer<CommandActionScript>
	{
	public:
		explicit CommandPlayerCopyOutfit(CommandList* const parent);

		void onClickScriptThread(Click& click) final;
	};
}
