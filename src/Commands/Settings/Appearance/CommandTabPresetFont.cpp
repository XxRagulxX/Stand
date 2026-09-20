#include "Commands/Settings/Appearance/CommandTabPresetFont.hpp"

namespace Stand::Features
{
	Stand::CommandTabPresetFont& GetCommandTabPresetFont()
	{
		static Stand::CommandTabPresetFont instance{};
		return instance;
	}
}
