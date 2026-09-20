#include "Commands/Settings/Appearance/CommandTabPosition.hpp"

namespace Stand::Features
{
	Stand::CommandTabPosition& GetCommandTabPosition()
	{
		static Stand::CommandTabPosition instance{};
		return instance;
	}
}
