#pragma once

#include <string>

#include "Util/hashtype.hpp"

namespace Stand
{
	[[nodiscard]] extern void joaatToStringInit();
	[[nodiscard]] extern void joaatToStringDeinit();

	[[nodiscard]] extern const char* joaatToStringRaw(const hash_t hash);
	[[nodiscard]] extern std::string joaatToString(const hash_t hash);
	[[nodiscard]] extern hash_t stringToJoaat(const std::string& str);
}
