#include "Commands/Settings/Appearance/CommandTabNotifySample.hpp"

namespace Stand::Features
{
	Stand::CommandTabNotifySample& GetCommandTabNotifySample()
	{
		static Stand::CommandTabNotifySample instance{};
		return instance;
	}
}
