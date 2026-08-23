#include "rlFriend.hpp"

#include "FunctionPointer.hpp"
#include "natives.hpp"

namespace rage
{
	int rlFriend::getCount()
	{
		return NETWORK::NETWORK_GET_FRIEND_COUNT();
	}

	rlFriend* rlFriend::get(int idx)
	{
		const auto sc_friend_offset = member_offset(&rlFriend::sc_friend);

		const auto name_offset = member_offset(&decltype(rlFriend::sc_friend)::name);

		const auto total_offset = sc_friend_offset + name_offset;

		return reinterpret_cast<rlFriend*>(reinterpret_cast<uintptr_t>(NETWORK::NETWORK_GET_FRIEND_NAME(idx)) - total_offset);
	}
}