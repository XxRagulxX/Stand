#include "Commands/Settings/Appearance/CommandTabCommands.hpp"

namespace Stand::Features
{
	Stand::CommandTabCommands& GetCommandTabCommands()
	{
		static Stand::CommandTabCommands instance{};
		return instance;
	}
}
