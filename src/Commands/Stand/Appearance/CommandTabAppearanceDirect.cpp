#include "Commands/Stand/Appearance/CommandTabAppearanceDirect.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabAppearanceDirect& GetCommandTabAppearanceDirect()
	{
		return *GetCommandTabStand().appearanceDirect;
	}
}
