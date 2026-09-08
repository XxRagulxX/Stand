#include "Menu/Items.hpp"
#include "Commands/PlayerCommand.hpp"
#include "Commands/Commands.hpp"

namespace Stand
{
	PlayerCommandItem::PlayerCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<PlayerCommand>(id)),
	    m_LabelOverride(label_override)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why.
	void PlayerCommandItem::Draw()
	{
	}
}
