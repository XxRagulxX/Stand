#include "Commands/Stand/Appearance/CommandTabBorder.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabBorder& GetCommandTabBorder()
	{
		return *GetCommandTabStand().border;
	}
}
