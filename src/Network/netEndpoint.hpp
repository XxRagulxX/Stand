#pragma once

#include "Network/netAddress.hpp"
#include "Network/netSocketAddress.hpp"

namespace rage
{
	class netEndpoint
	{
	public:
		char pad_0x000[0x0D0];
		netAddress relay_address;

		[[nodiscard]] const netSocketAddress& getRemoteAddress() const
		{
			return relay_address.proxy_sock_addr;
		}
	};
}
