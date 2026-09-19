#include "Commands/Self/Movement/CommandTabSuperFlight.hpp"

namespace Stand::Features
{
	Stand::CommandTabSuperFlight& GetCommandTabSuperFlight()
	{
		static Stand::CommandTabSuperFlight instance{};
		return instance;
	}
}
