#pragma once

#include <soup/gmBoxCorners.hpp>

#include "Game/fwddecl.hpp"

namespace Stand
{
	struct Box : public soup::gmBoxCorners
	{
		using soup::gmBoxCorners::gmBoxCorners;
	};
}
