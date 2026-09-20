#include "Commands/Settings/Appearance/CommandTabHeader.hpp"

namespace Stand
{
	// Declared here (not as a createChild<> member) because CommandListSelect
	// derives from CommandLegacy, which is a separate hierarchy from Command —
	// CommandList::createChild<T> expects T to derive from Command and would
	// fail to compile. Static storage auto-calls CommandLegacy's constructor,
	// which registers the instance under "header"_J via Commands::AddCommand.
	static CommandHeaderMode g_HeaderMode{nullptr};
}

namespace Stand::Features
{
	Stand::CommandTabHeader& GetCommandTabHeader()
	{
		static Stand::CommandTabHeader instance{};
		return instance;
	}
}
