#include "Commands/Settings/CommandTabPosition.hpp"

namespace Stand::Features
{
	Stand::CommandTabPosition& GetCommandTabPosition()
	{
		static Stand::CommandTabPosition instance{};
		return instance;
	}
}
