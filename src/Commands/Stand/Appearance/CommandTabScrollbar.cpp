#include "Commands/Stand/Appearance/CommandTabScrollbar.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabScrollbar& GetCommandTabScrollbar()
	{
		return *GetCommandTabStand().scrollbar;
	}
}
