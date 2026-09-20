#include "Commands/Settings/Appearance/CommandTabScrollbar.hpp"

namespace Stand::Features
{
	Stand::CommandTabScrollbar& GetCommandTabScrollbar()
	{
		static Stand::CommandTabScrollbar instance{};
		return instance;
	}
}
