#include "Weapons/set_aim_mode.hpp"

#include "Game/natives.hpp"

namespace Stand
{
	void set_aim_mode(int aim_mode)
	{
		PLAYER::SET_PLAYER_TARGETING_MODE(aim_mode);
	}
}
