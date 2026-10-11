#include "Commands/Widgets/CommandListTextSettings.hpp"

#include "Rendering/StandPort/CommandLambdaSlider.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/StandPort/NotifyGrid.hpp"
#include "Rendering/StandPort/TextSettings.hpp"

namespace Stand
{
	static constexpr float TEXT_HEIGHT_PX = 0.021875f;

	CommandListTextSettings::CommandListTextSettings(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, TextSettings* settings)
		: CommandList(parent, std::move(menu_name), std::move(command_names))
	{
		createChild<CommandLambdaSlider<CommandSlider>>(LOC("SCLE"), getSuffixedCommandNames(CMDNAME("scale")), NOLABEL, 1, SHRT_MAX, int(settings->scale / TEXT_HEIGHT_PX / 2.0f), 1, [settings](int value, Click& click)
		{
			if (g_gui.canUpdateThemeSettingWithDefaultDependency())
			{
				settings->scale = float(value) * TEXT_HEIGHT_PX * 2.0f;
				g_notify_grid.update();
			}
		});
		createChild<CommandLambdaSlider<CommandSlider>>(LOC("OFF_X"), getSuffixedCommandNames(CMDNAME("xoffset")), NOLABEL, -15, 15, (int)settings->x_offset, 1, [settings](int value, Click& click)
		{
			if (g_gui.canUpdateThemeSettingWithDefaultDependency())
			{
				settings->x_offset = (float)value;
			}
		});
		createChild<CommandLambdaSlider<CommandSlider>>(LOC("OFF_Y"), getSuffixedCommandNames(CMDNAME("yoffset")), NOLABEL, -10, 10, (int)settings->y_offset, 1, [settings](int value, Click& click)
		{
			if (g_gui.canUpdateThemeSettingWithDefaultDependency())
			{
				settings->y_offset = (float)value;
			}
		});
	}
}
