#include "Commands/Settings/Appearance/CommandTabCursor.hpp"

namespace Stand::Features
{
	Stand::CommandTabCursor& GetCommandTabCursor()
	{
		static Stand::CommandTabCursor instance{};
		return instance;
	}
}
