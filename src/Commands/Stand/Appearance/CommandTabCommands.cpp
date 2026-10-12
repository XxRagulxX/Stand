#include "Commands/Stand/Appearance/CommandTabCommands.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabCommands& GetCommandTabCommands()
	{
		return *GetCommandTabStand().commands;
	}
}
