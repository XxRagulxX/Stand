#include "Commands/Stand/Input/CommandTabInput.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabInput& GetCommandTabInput()
	{
		return *GetCommandTabStand().input;
	}
}
