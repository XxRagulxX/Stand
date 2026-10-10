#pragma once

#include "Network/CMultiplayerChat.hpp"

namespace pointers
{
	inline CMultiplayerChat* chat_box_instance{};
	inline CMultiplayerChat** chat_box = &chat_box_instance;
}
