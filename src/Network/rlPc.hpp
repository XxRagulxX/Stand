#pragma once

#include "Game/gta_fwddecl.hpp"
#include "Util/struct_base.hpp"

namespace rage
{
	struct rlPc
	{
		PAD(0, 0x60) rgsc::rlPcProfileManager* profile_manager;
		PAD(0x68, 0x78) rgsc::IPlayerManagerV1* player_manager;
		/* 0x80 */ rgsc::IPresenceManagerV1* presence_manager;
	};
	static_assert(sizeof(rlPc) == 0x88);
}
