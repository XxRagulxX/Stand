#include "Menu/Items.hpp"
#include "Commands/Commands.hpp"
#include "Commands/CommandInput.hpp"

namespace Stand
{
	StringCommandItem::StringCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<CommandInput>(id)),
	    m_LabelOverride(label_override)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why.
	void StringCommandItem::Draw()
	{
	}
}
