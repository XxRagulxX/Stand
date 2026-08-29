#include "Rendering/create_rainbow.hpp"

#include <fmt/core.h>

#include "Commands/Widgets/CommandColourCustom.hpp"
#include "Commands/Stand/CommandRainbow.hpp"

namespace Stand
{
	void create_rainbow(CommandColourCustom* colour)
	{
		colour->children.insert(colour->children.begin(), colour->makeChild<CommandRainbow>(LOC("LGBT"), colour->getRainbowCommandNames(), colour));
	}
}
