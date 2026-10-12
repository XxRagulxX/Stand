#include "Commands/Stand/Appearance/CommandTabCommandInfoText.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabCommandInfoText& GetCommandTabCommandInfoText()
	{
		return *GetCommandTabStand().commandInfoText;
	}
}
