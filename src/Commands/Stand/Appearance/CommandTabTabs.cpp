#include "Commands/Stand/Appearance/CommandTabTabs.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabTabs& GetCommandTabTabs()
	{
		return *GetCommandTabStand().tabs;
	}
}
