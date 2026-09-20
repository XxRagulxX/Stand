#include "Commands/Settings/Appearance/CommandTabEntityPreviews.hpp"

namespace Stand::Features
{
	Stand::CommandTabEntityPreviews& GetCommandTabEntityPreviews()
	{
		static Stand::CommandTabEntityPreviews instance{};
		return instance;
	}
}
