#include "Commands/Stand/Appearance/CommandTabAddressBar.hpp"

namespace Stand::Features
{
	Stand::CommandTabAddressBar& GetCommandTabAddressBar()
	{
		static Stand::CommandTabAddressBar instance{};
		return instance;
	}
}
