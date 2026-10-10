#pragma once

#include <cstdint>
#include <string>

namespace Stand
{
	class Chat
	{
	public:
		static void addToDraft(const std::wstring&) {}
		static void removeFromDraft(uint16_t) {}
	};
}
