#pragma once

#include "Network/rlSessionInfo.hpp"
#include "Network/SessionAttributes.hpp"

namespace Stand
{
#pragma pack(push, 1)
	struct Session
	{
		rage::rlSessionInfo info;
		SessionAttributes attrs;
	};
#pragma pack(pop)
}
