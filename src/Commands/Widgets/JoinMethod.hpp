#pragma once

namespace Stand
{
	enum JoinMethod : long long
	{
		JM_NUTS,
		JM_NINJA,
		JM_MATCHMAKING,
		JM_FRIEND,

		JM_NUTS_REJOIN,

		JM_DEFAULT = JM_NINJA
	};
}
