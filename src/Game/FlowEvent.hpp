#pragma once

#include <cstdint>

namespace Stand
{
	struct FlowEvent
	{
		enum Id : uint32_t
		{
			MOD_NONET = 0,
			MISC_HOSTCHANGE,
			MISC_SCRIPTHOSTCHANGE,
			SIZE
		};
	};
}
