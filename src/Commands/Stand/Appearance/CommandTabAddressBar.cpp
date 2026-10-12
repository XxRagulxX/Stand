#include "Commands/Stand/Appearance/CommandTabAddressBar.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabAddressBar& GetCommandTabAddressBar()
	{
		return *GetCommandTabStand().addressBar;
	}
}
