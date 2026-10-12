#include "Commands/Stand/Appearance/CommandTabAppearanceDirect.hpp"

namespace Stand::Features
{
	Stand::CommandTabAppearanceDirect& GetCommandTabAppearanceDirect()
	{
		static Stand::CommandTabAppearanceDirect instance{};
		return instance;
	}
}
