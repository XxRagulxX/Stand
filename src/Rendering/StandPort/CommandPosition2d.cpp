#include "Rendering/StandPort/CommandPosition2d.hpp"

#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/StandPort/CommandLambdaSlider.hpp"
#include "Menu/Click.hpp"

#include <climits>

namespace Stand
{
	CommandPosition2d::CommandPosition2d(CommandList* parent, Label&& menu_name, const CommandName& command_name, const Rendering::Position2d& default_pos, Label&& help_text)
		: CommandList(parent, std::move(menu_name), {}, std::move(help_text))
	{
		x = createChild<CommandLambdaSlider<CommandSlider>>(LIT("X"), std::vector<CommandName>{ CommandName(command_name).append(CMDNAME("x")) }, NOLABEL, SHRT_MIN, SHRT_MAX, (int)default_pos.x, 10, [this](int, Click&)
		{
			onChange(getPosition());
		});
		y = createChild<CommandLambdaSlider<CommandSlider>>(LIT("Y"), std::vector<CommandName>{ CommandName(command_name).append(CMDNAME("y")) }, NOLABEL, SHRT_MIN, SHRT_MAX, (int)default_pos.y, 10, [this](int, Click&)
		{
			onChange(getPosition());
		});
		mouse = createChild<CommandPosition2dMouse>(command_name);
	}

	Rendering::Position2d CommandPosition2d::getPosition() const
	{
		return { static_cast<int16_t>(x->value), static_cast<int16_t>(y->value) };
	}

	void CommandPosition2d::onBack(ThreadContext thread_context)
	{
		if (mouse && mouse->m_on)
		{
			Click click(CLICK_AUTO, thread_context);
			mouse->setStateBool(click, false);
		}
		CommandList::onBack(thread_context);
	}
}
