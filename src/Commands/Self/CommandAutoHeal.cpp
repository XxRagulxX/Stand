#include "Commands/Self/CommandAutoHeal.hpp"

// See Commands/Self/CommandGod.cpp's own comment for why this is a
// function-local static behind an accessor (parent=nullptr - no "Self"
// Stand::CommandList root exists anywhere in this codebase yet) rather
// than a plain namespace-scope global.
namespace Stand::Features
{
	Stand::CommandAutoHeal& GetCommandAutoHeal()
	{
		static Stand::CommandAutoHeal instance{nullptr};
		return instance;
	}
}
