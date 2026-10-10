#pragma once

#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct CMultiplayerChat
{
	int32_t iHideChatWindowCount = 0;
	int32_t iChatBackCount = 0;
	int32_t message_length = 0;
	int32_t last_delete_tick = 0;
	std::wstring message{};
};
#pragma pack(pop)
