#include "Commands/Settings/CommandTabTextures.hpp"

namespace Stand::Features
{
	Stand::CommandTabTextures& GetCommandTabTextures()
	{
		static Stand::CommandTabTextures instance{};
		return instance;
	}
}
