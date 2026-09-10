#include "Commands/Settings/CommandTabBorder.hpp"

namespace Stand::Features
{
	Stand::CommandTabBorder& GetCommandTabBorder()
	{
		static Stand::CommandTabBorder instance{};
		return instance;
	}
}
