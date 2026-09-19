#include "Commands/Self/Weapons/CommandTabWeapons.hpp"

namespace Stand::Features
{
	Stand::CommandTabWeapons& GetCommandTabWeapons()
	{
		static Stand::CommandTabWeapons instance{};
		return instance;
	}
}
