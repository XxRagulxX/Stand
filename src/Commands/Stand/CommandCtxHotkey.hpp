#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandCtxHotkey : public CommandList
	{
	public:
		Hotkey* const hotkey;

		explicit CommandCtxHotkey(CommandList* parent, Hotkey* hotkey, CommandPhysical* target);

		void save(CommandPhysical* target);
	};
}
