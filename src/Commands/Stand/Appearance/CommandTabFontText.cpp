#include "Commands/Stand/Appearance/CommandTabFontText.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabFontText& GetCommandTabFontText()
	{
		return *GetCommandTabStand().fontText;
	}
}
