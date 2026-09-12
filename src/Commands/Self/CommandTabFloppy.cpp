#include "Commands/Self/CommandTabFloppy.hpp"

namespace Stand::Features
{
	Stand::CommandTabFloppy& GetCommandTabFloppy()
	{
		static Stand::CommandTabFloppy instance{};
		return instance;
	}
}
