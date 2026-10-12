#include "Commands/Stand/Appearance/CommandTabTabs.hpp"

namespace Stand::Features
{
	Stand::CommandTabTabs& GetCommandTabTabs()
	{
		static Stand::CommandTabTabs instance{};
		return instance;
	}
}
