#include "Network/netEndpoint.hpp"

#include "Network/CNetworkSession.hpp"
#include "Network/IceQueen.hpp"
#include "Network/netConnectionManager.hpp"
#include "Network/NetworkSessionMessages.hpp"
#include "Game/pointers.hpp"
#include "Network/rlGamerId.hpp"
#include "Network/snet_messages.hpp"

#ifdef STAND_DEBUG
#include "Core/FileLogger.hpp"
#endif

namespace rage
{
	using namespace Stand;

	const netSocketAddress& netEndpoint::getPublicAddress() const
	{
		const netSocketAddress& p2p_addr = getRemoteAddress();
		if (p2p_addr.ip == -1)
		{
			if (auto iqp = IceQueen::findPeer(GetPeerId()))
			{
				return iqp->public_addr;
			}
		}
		return p2p_addr;
	}

#ifdef STAND_DEBUG
	void netEndpoint::sanityCheck() const
	{
		SOUP_IF_UNLIKELY (m_ByEndpointIdLink.m_key != id)
		{
			g_logger.log("rage::netEndpoint is wrong: m_ByEndpointIdLink.m_key != id");
		}
	}
#endif

	void netEndpoint::removeGamer(uint32_t channelId, uint64_t sessionId, const rlGamerHandle& target, int remove_reason) const
	{
		snMsgRemoveGamersFromSessionCmd msg;
		msg.session_id = sessionId;
		msg.targets[0] = target;
		msg.remove_reason = remove_reason;
		msg.num_targets = 1;
		(*pointers::connection_mgr)->SendOutOfBand(GetAddress(), channelId, msg, 0);
	}
}
