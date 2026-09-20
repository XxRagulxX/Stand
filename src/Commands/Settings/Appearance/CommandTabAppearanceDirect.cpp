#include "Commands/Settings/Appearance/CommandTabAppearanceDirect.hpp"

namespace Stand::Features
{
	Stand::CommandTabAppearanceDirect& GetCommandTabAppearanceDirect()
	{
		static Stand::CommandTabAppearanceDirect instance{};
		return instance;
	}
}
