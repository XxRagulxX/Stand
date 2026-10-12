#include "Commands/Stand/Appearance/CommandTabPresetFont.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabPresetFont& GetCommandTabPresetFont()
	{
		return *GetCommandTabStand().presetFont;
	}
}
