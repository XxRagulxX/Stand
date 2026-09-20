#include "Commands/Settings/CommandTabFontText.hpp"

namespace Stand::Features
{
	Stand::CommandTabFontText& GetCommandTabFontText()
	{
		static Stand::CommandTabFontText instance{};
		return instance;
	}
}
