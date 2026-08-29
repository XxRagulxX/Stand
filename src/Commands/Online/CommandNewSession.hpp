#pragma once

#include "Commands/Stand/CommandListNameshare.hpp"

#include "Network/SessionType.hpp"

namespace Stand
{
	class CommandNewSession : public CommandListNameshare
	{
	private:
		CommandToggleNoCorrelation* select_character;

	public:
		explicit CommandNewSession(CommandList* const parent);

		void onChildClick(Click& click, SessionType value);
	};
}
