#pragma once
#include <cstdint>

class MatchmakingId
{
public:
	char m_data1[0x26];
	std::uint64_t m_data2;
	std::uint64_t m_data3;
};