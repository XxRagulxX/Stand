#include "Commands/Settings/CommandTabCursor.hpp"

namespace Stand::Features
{
	Stand::CommandTabCursor& GetCommandTabCursor()
	{
		static Stand::CommandTabCursor instance{};
		return instance;
	}
}
