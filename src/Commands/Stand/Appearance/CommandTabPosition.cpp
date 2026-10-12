#include "Commands/Stand/Appearance/CommandTabPosition.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabPosition& GetCommandTabPosition()
	{
		return *GetCommandTabStand().position;
	}
}
