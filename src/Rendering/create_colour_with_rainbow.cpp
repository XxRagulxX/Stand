#include "Rendering/create_colour_with_rainbow.hpp"

#include "Commands/Widgets/CommandColourPointer.hpp"
#include "Commands/Widgets/CommandColourPointerDirectx.hpp"
#include "Rendering/create_rainbow.hpp"

namespace Stand
{
	void create_colour_with_rainbow(CommandList* list, DirectX::SimpleMath::Color* colour, std::vector<CommandName>&& command_names)
	{
		create_rainbow(list->createChild<CommandColourPointerDirectx>(colour, LOC("CLR"), std::move(command_names)));
	}

	void create_colour_with_rainbow(CommandList* list, Colour* colour, std::vector<CommandName>&& command_names)
	{
		create_rainbow(list->createChild<CommandColourPointer>(colour, LOC("CLR"), std::move(command_names)));
	}
}
