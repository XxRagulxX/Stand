#include "Menu/Items.hpp"
#include "Commands/CommandColourCustom.hpp"
#include "Commands/Commands.hpp"

namespace Stand
{
	ColorCommandItem::ColorCommandItem(joaat_t id, std::optional<std::string> labelOverride) :
	    m_Command(Commands::GetCommand<CommandColourCustom>(id)),
	    m_LabelOverride(labelOverride)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why.
	void ColorCommandItem::Draw()
	{
	}
}
