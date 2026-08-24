#pragma once

#include <string>

#include "hashtype.hpp"

namespace Stand
{
    extern void joaatToStringInit(); // Internal use only
    extern void joaatToStringDeinit(); // Internal use only

	[[nodiscard]] extern const char* joaatToStringRaw(const hash_t hash);
	[[nodiscard]] extern std::string joaatToString(const hash_t hash);
	[[nodiscard]] extern hash_t stringToJoaat(const std::string& str);
}
