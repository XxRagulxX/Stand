#include "Commands/Stand/Appearance/CommandTabNotifyTiming.hpp"

namespace Stand::Features
{
	Stand::CommandTabNotifyTiming& GetCommandTabNotifyTiming()
	{
		static Stand::CommandTabNotifyTiming instance{};
		return instance;
	}
}
