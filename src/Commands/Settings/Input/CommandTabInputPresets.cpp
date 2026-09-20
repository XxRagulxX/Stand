#include "Commands/Settings/Input/CommandTabInputPresets.hpp"

namespace Stand::Features
{
	Stand::CommandTabInputPresets& GetCommandTabInputPresets()
	{
		static Stand::CommandTabInputPresets instance{};
		return instance;
	}
}
