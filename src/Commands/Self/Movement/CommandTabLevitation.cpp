#include "Commands/Self/Movement/CommandTabLevitation.hpp"

namespace Stand::Features
{
	Stand::CommandTabLevitation& GetCommandTabLevitation()
	{
		static Stand::CommandTabLevitation instance{};
		return instance;
	}
}
