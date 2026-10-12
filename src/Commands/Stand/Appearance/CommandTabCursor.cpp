#include "Commands/Stand/Appearance/CommandTabCursor.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabCursor& GetCommandTabCursor()
	{
		return *GetCommandTabStand().cursor;
	}
}
