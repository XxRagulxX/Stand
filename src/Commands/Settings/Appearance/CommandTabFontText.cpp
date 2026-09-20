#include "Commands/Settings/Appearance/CommandTabFontText.hpp"

namespace Stand::Features
{
	Stand::CommandTabFontText& GetCommandTabFontText()
	{
		static Stand::CommandTabFontText instance{};
		return instance;
	}
}
