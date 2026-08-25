#include "Core/ResourceMgr.hpp"

#include "Network/AddressGamers.hpp"
#include "Network/ChatHistory.hpp"
#include "Network/RemoteGamer.hpp"
#include "Network/ScAccount.hpp"

namespace Stand
{
	void ResourceMgr::deinit()
	{
		AddressGamers::deinit();
		ChatHistory::history.clear();
		RemoteGamer::deinit();
		ScAccount::deinit();
	}
}
