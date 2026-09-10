#include "Commands/Settings/CommandTabCommands.hpp"

namespace Stand::Features
{
	Stand::CommandTabCommands& GetCommandTabCommands()
	{
		static Stand::CommandTabCommands instance{};
		return instance;
	}
}
