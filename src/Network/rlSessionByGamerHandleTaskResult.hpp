#pragma once
#include "Network/rlGamerHandle.hpp"
#include "Network/rlSessionInfo.hpp"

namespace rage
{
	class rlSessionByGamerTaskResult
	{
	public:
		rlGamerHandle m_GamerHandle{0};
		rlSessionInfo m_SessionInfo{};
	};
}