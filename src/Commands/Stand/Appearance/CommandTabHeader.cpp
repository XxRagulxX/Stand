#include "Commands/Stand/Appearance/CommandTabHeader.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand
{
	static CommandHeaderMode g_HeaderMode{nullptr};
}

namespace Stand::Features
{
	Stand::CommandTabHeader& GetCommandTabHeader()
	{
		return *GetCommandTabStand().header;
	}
}
