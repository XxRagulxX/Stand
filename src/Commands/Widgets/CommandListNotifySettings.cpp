#include "Commands/Widgets/CommandListNotifySettings.hpp"

#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Rendering/StandPort/CommandLambdaSlider.hpp"
#include "Rendering/StandPort/CommandSliderPointer.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/StandPort/GridToaster.hpp"
#include "Rendering/StandPort/NotifyGrid.hpp"
#include "Util/Toaster.hpp"

#include "Commands/Widgets/CommandNotifyType.hpp"
#include "Commands/Widgets/CommandNotifyPosition.hpp"
#include "Commands/Widgets/CommandNotifyInvertFlow.hpp"
#include "Commands/Widgets/CommandNotifyBorderColour.hpp"
#include "Commands/Widgets/CommandNotifyFlashColour.hpp"
#include "Commands/Widgets/CommandNotifyBgColour.hpp"

namespace Stand
{
	CommandListNotifySettings::CommandListNotifySettings(CommandList* parent, CommandColourCustom* primaryColour, CommandColourCustom* bgColour)
		: CommandList(parent, LOC("NOTFS"))
	{
		auto* type = createChild<CommandNotifyType>();
		type->custom_pos = createChild<CommandNotifyPosition>();
		createChild<CommandNotifyInvertFlow>();
		createChild<CommandLambdaSlider<CommandSlider>>(LOC("WDT"), CMDNAMES("notifywidth"), NOLABEL, 0, 15000, 400, 10, [](int value, Click& click)
		{
			g_notify_grid.updateSetting([value]
			{
				g_notify_grid.notify_width = value;
			});
		});
		createChild<CommandLambdaSlider<CommandSlider>>(LOC("PDDGN"), CMDNAMES("notifypadding"), NOLABEL, 0, 15000, 10, 1, [](int value, Click& click)
		{
			g_notify_grid.updateSetting([value]
			{
				g_notify_grid.notify_padding = value;
			});
		});
		createChild<CommandNotifyBorderColour>(primaryColour);
		createChild<CommandNotifyFlashColour>();
		createChild<CommandNotifyBgColour>(bgColour);
		createChild<CommandLambdaAction>(LOC("FLSHNTF"), std::vector<CommandName>{}, NOLABEL, [](Click&)
		{
			GridToaster::flashPersistentToast();
		});

		timing = createChild<CommandList>(LOC("TIMNG"));
		timing->createChild<CommandSliderPointer<CommandSlider, uint16_t>>(LOC("READSPD"), CMDNAMES("readspeed", "readingspeed"), NOLABEL, &g_grid_toaster.reading_speed_wpm, 0, 1000, 10);
		timing->createChild<CommandSliderPointer<CommandSlider, time_t>>(LOC("READSTRT"), CMDNAMES("readstartdelay", "readingstartdelay"), NOLABEL, &g_grid_toaster.reading_start_delay, 0, 1000, 50);
		timing->createChild<CommandSliderPointer<CommandSlider, time_t>>(LOC("MINDUR"), CMDNAMES("notifyminduration"), NOLABEL, &g_grid_toaster.notifydur_min, 0, 60000, 100);
		timing->createChild<CommandSliderPointer<CommandSlider, time_t>>(LOC("MAXDUR"), CMDNAMES("notifymaxduration"), NOLABEL, &g_grid_toaster.notifydur_max, 0, 60000, 100);
	}

	void CommandListNotifySettings::onActiveListUpdate()
	{
	}

	void CommandListNotifySettings::setToaster(ThreadContext thread_context, Toaster* toaster)
	{
		g_toaster = toaster;
	}

	void CommandListNotifySettings::updateGameColour(ThreadContext thread_context)
	{
	}
}
