#pragma once

#include "Rendering/StandPort/dx_common.hpp"

#include <SimpleMath.h>
#include <vector>

#include "Commands/Online/CommandName.hpp"

namespace Stand
{
	class CommandList;
	extern void create_colour_with_rainbow(CommandList* list, DirectX::SimpleMath::Color* colour, std::vector<CommandName>&& command_names);
}
