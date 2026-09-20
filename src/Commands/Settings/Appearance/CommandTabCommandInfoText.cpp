#include "Commands/Settings/CommandTabCommandInfoText.hpp"

namespace Stand::Features
{
	Stand::CommandTabCommandInfoText& GetCommandTabCommandInfoText()
	{
		static Stand::CommandTabCommandInfoText instance{};
		return instance;
	}
}
