#include "Rendering/StandPort/create_rainbow.hpp"

#include <fmt/core.h>

#include "Rendering/StandPort/CommandColourCustom.hpp"
#include "Commands/Widgets/CommandRainbow.hpp"

namespace Stand
{
	void create_rainbow(CommandColourCustom* colour)
	{
		colour->children.insert(colour->children.begin(), colour->makeChild<CommandRainbow>(LOC("LGBT"), colour->getRainbowCommandNames(), colour));
	}
}
