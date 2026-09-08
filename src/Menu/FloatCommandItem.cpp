#include "Menu/Items.hpp"
#include "Commands/Commands.hpp"
#include "Commands/CommandSliderFloatLegacy.hpp"

namespace Stand
{
	FloatCommandItem::FloatCommandItem(joaat_t id, std::optional<std::string> label_override, bool use_slider) :
	    m_useSlider(use_slider),
	    m_Command(Commands::GetCommand<CommandSliderFloatLegacy>(id)),
	    m_LabelOverride(label_override)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why.
	void FloatCommandItem::Draw()
	{
	}
}
