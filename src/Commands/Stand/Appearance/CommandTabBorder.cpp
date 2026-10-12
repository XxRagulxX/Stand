#include "Commands/Stand/Appearance/CommandTabBorder.hpp"

namespace Stand::Features
{
	Stand::CommandTabBorder& GetCommandTabBorder()
	{
		static Stand::CommandTabBorder instance{};
		return instance;
	}
}
