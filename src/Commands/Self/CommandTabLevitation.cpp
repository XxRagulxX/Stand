#include "Commands/Self/CommandTabLevitation.hpp"

namespace Stand::Features
{
	Stand::CommandTabLevitation& GetCommandTabLevitation()
	{
		static Stand::CommandTabLevitation instance{};
		return instance;
	}
}
