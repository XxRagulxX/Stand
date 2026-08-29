#pragma once

#include <deque>

#include "Game/natives_decl.hpp"

namespace Stand
{
	struct Firemangr
	{
		inline static std::deque<FireId> script_fires{};
	};
}
