#include "Commands/Settings/CommandTabAddressBar.hpp"

namespace Stand::Features
{
	Stand::CommandTabAddressBar& GetCommandTabAddressBar()
	{
		static Stand::CommandTabAddressBar instance{};
		return instance;
	}
}
