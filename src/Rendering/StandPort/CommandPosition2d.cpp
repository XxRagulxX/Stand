#include "Rendering/StandPort/CommandPosition2d.hpp"

#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/StandPort/CommandLambdaSlider.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	CommandPosition2d::CommandPosition2d(CommandList* parent, Label&& menu_name, const CommandName& command_name, const Rendering::Position2d& default_pos, Label&& help_text)
		: CommandList(parent, std::move(menu_name), {}, std::move(help_text))
	{
		x = createChild<CommandLambdaSlider<CommandSlider>>(LIT("X"), std::vector<CommandName>{}, NOLABEL, -5000, 5000, (int)default_pos.x, 1, [this](int, Click&)
		{
			onChange(getPosition());
		});
		y = createChild<CommandLambdaSlider<CommandSlider>>(LIT("Y"), std::vector<CommandName>{}, NOLABEL, -5000, 5000, (int)default_pos.y, 1, [this](int, Click&)
		{
			onChange(getPosition());
		});
	}

	Rendering::Position2d CommandPosition2d::getPosition() const
	{
		return { static_cast<int16_t>(x->value), static_cast<int16_t>(y->value) };
	}
}
