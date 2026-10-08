#pragma once

#include "Network/rlGamerInfo.hpp"

#pragma pack(push, 1)
class CPlayerInfo
{
public:
	char pad_0x00[0x20];
	rage::rlGamerInfo gamer_info;
	char pad_after_gamer_info[0x1C8 - 0x20 - (int)sizeof(rage::rlGamerInfo)];
	float m_swim_speed;
	char pad_0x1CC[0x240 - 0x1CC];
	CPed* m_ped;
};
#pragma pack(pop)
