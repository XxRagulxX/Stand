#pragma once

#include "Network/netMessageText.hpp"

#include "Network/rlGamerHandle.hpp"

struct MsgTextMessage : public rage::netMessageText
{
	/* 256 */ rage::rlGamerHandle sender;
};
static_assert(sizeof(MsgTextMessage) == 256 + sizeof(rage::rlGamerHandle));
