#include "Rendering/StandPort/create_colour_with_rainbow.hpp"

#include "Commands/Widgets/CommandColourPointerDirectx.hpp"
#include "Rendering/StandPort/create_rainbow.hpp"
#include "Util/Label.hpp"
#include "Util/lang.hpp"

namespace Stand
{
	void create_colour_with_rainbow(CommandList* list, DirectX::SimpleMath::Color* colour, std::vector<CommandName>&& command_names)
	{
		create_rainbow(list->createChild<CommandColourPointerDirectx>(colour, LOC("CLR"), std::move(command_names)));
	}
}
