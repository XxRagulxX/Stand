#pragma once

#include <string>

#include "Game/typedecl.hpp"

namespace Stand
{
	struct TimedText
	{
		time_t offset_millis;
		std::string text;
	};
}
