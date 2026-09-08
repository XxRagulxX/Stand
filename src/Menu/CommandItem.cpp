#include "Menu/Items.hpp"
#include "Commands/Commands.hpp"
#include "Commands/CommandLegacy.hpp"

namespace Stand
{
	CommandItem::CommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<CommandLegacy>(id)),
	    m_LabelOverride(label_override)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why (this whole
	// classic tree has no renderer left; only reachable at all via a
	// Group's own :draw() from Lua).
	void CommandItem::Draw()
	{
	}
}
