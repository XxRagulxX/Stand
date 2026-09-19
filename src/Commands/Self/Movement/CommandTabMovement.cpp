#include "Commands/Self/Movement/CommandTabMovement.hpp"

namespace Stand::Features
{
	Stand::CommandTabMovement& GetCommandTabMovement()
	{
		static Stand::CommandTabMovement instance{};
		return instance;
	}
}
