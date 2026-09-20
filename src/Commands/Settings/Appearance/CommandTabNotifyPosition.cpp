#include "Commands/Settings/Appearance/CommandTabNotifyPosition.hpp"

namespace Stand::Features
{
	Stand::CommandTabNotifyPosition& GetCommandTabNotifyPosition()
	{
		static Stand::CommandTabNotifyPosition instance{};
		return instance;
	}
}
