#include "Commands/Settings/Appearance/CommandTabNotifyTiming.hpp"

namespace Stand::Features
{
	Stand::CommandTabNotifyTiming& GetCommandTabNotifyTiming()
	{
		static Stand::CommandTabNotifyTiming instance{};
		return instance;
	}
}
