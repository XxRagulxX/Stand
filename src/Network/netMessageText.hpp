#pragma once

#include <cstring>

#include "Network/netMessage.hpp"

namespace rage
{
	struct netMessageText : public netMessage
	{
		char text[256]{};

		void setText(const char* source) noexcept
		{
			const size_t length = strnlen(source, sizeof(text) - 1);

			std::memcpy(text, source, length);
			text[length] = '\0';
		}
	};
	static_assert(sizeof(netMessageText) == 256);
}