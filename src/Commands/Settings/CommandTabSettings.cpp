#include "Commands/Settings/CommandTabSettings.hpp"

namespace Stand::Features
{
	Stand::CommandTabSettings& GetCommandTabSettings()
	{
		static Stand::CommandTabSettings instance{};
		return instance;
	}
}
