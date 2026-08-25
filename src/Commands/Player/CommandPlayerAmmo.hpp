#pragma once

#include "Commands/Player/CommandOnPlayer.hpp"
#include "Commands/Extra/CommandActionScript.hpp"

namespace Stand
{
	class CommandPlayerAmmo : public CommandOnPlayer<CommandActionScript>
	{
	public:
		explicit CommandPlayerAmmo(CommandList* const parent);

		void onClickScriptThread(Click& click) final;
	};
}
