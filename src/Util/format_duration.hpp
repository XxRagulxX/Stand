#pragma once

#include <string>

#include "Game/fwddecl.hpp"

namespace Stand
{
	[[nodiscard]] extern std::string format_duration(time_t ms);
}
