#include "Commands/Self/CommandTabSuperFlight.hpp"

namespace Stand::Features
{
	Stand::CommandTabSuperFlight& GetCommandTabSuperFlight()
	{
		static Stand::CommandTabSuperFlight instance{};
		return instance;
	}
}
