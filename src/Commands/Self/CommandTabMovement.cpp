#include "Commands/Self/CommandTabMovement.hpp"

namespace Stand::Features
{
	Stand::CommandTabMovement& GetCommandTabMovement()
	{
		static Stand::CommandTabMovement instance{};
		return instance;
	}
}
