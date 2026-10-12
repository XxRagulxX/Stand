#include "Commands/Stand/Appearance/CommandTabTextures.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabTextures& GetCommandTabTextures()
	{
		return *GetCommandTabStand().textures;
	}
}
