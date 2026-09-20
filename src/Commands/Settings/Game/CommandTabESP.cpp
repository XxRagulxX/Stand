#include "Commands/Settings/Game/CommandTabESP.hpp"

namespace Stand::Features
{
	Stand::CommandTabESP& GetCommandTabESP()
	{
		static Stand::CommandTabESP instance{};
		return instance;
	}
}
