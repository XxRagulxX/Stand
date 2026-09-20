#include "Commands/Settings/Appearance/CommandTabCommandInfoText.hpp"

namespace Stand::Features
{
	Stand::CommandTabCommandInfoText& GetCommandTabCommandInfoText()
	{
		static Stand::CommandTabCommandInfoText instance{};
		return instance;
	}
}
