#pragma once

#include "Network/netMessageText.hpp"
#include "Network/netMessageId.hpp"

namespace rage
{
	struct CMsgTextMessage : public netMessageText
	{
		AUTOID_MSG(CMSG_TEXT_MESSAGE);

		/* 256 */ char unk_uuid[40]; // e.g. "c1aba3ff-0c9a-1209-92ec-3e70e30a058c"
		/* 296 */ rlGamerHandle sender;
		/* 312 */ bool team_chat;

		/*template <typename T>
		bool ser(T& bb)
		{
			return bb.serStr(text, 256)
				&& bb.serU64(sender.data)
				&& bb.serBool(team_chat)
				;
		}*/
	};
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif
	static_assert(offsetof(CMsgTextMessage, sender) == 296);
	static_assert(offsetof(CMsgTextMessage, team_chat) == 312);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
}
