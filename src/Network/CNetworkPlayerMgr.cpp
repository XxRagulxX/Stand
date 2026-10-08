#include "Network/CNetworkPlayerMgr.hpp"

compactplayer_t CNetworkPlayerMgr::getHostIndex() const
{
	compactplayer_t p = 0;
	for (; p != MAX_PLAYERS; ++p)
	{
		if (Players[p] != nullptr && Players[p]->IsHost())
		{
			break;
		}
	}
	return p;
}
