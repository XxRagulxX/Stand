#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/NotifySettings.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandNotifyReadSpeed : public CommandSlider
	{
	public:
		explicit CommandNotifyReadSpeed(CommandList* const parent)
			: CommandSlider(parent, LIT("Reading Speed (WPM)"), CMDNAMES("readspeed"),
				NOLABEL, 0, 1000, 250, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kReadingSpeedWpm = static_cast<uint16_t>(value > 0 ? value : 0);
		}
	};

	class CommandNotifyReadStartDelay : public CommandSlider
	{
	public:
		explicit CommandNotifyReadStartDelay(CommandList* const parent)
			: CommandSlider(parent, LIT("Reading Start Delay (ms)"), CMDNAMES("readstartdelay"),
				NOLABEL, 0, 5000, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kReadingStartDelayMs = value;
		}
	};

	class CommandNotifyMinDuration : public CommandSlider
	{
	public:
		explicit CommandNotifyMinDuration(CommandList* const parent)
			: CommandSlider(parent, LIT("Min Duration (ms)"), CMDNAMES("notifyminduration"),
				NOLABEL, 0, 60000, 2000, 100)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kMinDurationMs = value;
		}
	};

	class CommandNotifyMaxDuration : public CommandSlider
	{
	public:
		explicit CommandNotifyMaxDuration(CommandList* const parent)
			: CommandSlider(parent, LIT("Max Duration (ms)"), CMDNAMES("notifymaxduration"),
				NOLABEL, 0, 60000, 10000, 100)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kMaxDurationMs = value;
		}
	};

	class CommandTabNotifyTiming : public CommandList
	{
	public:
		CommandNotifyReadSpeed* const readSpeed;
		CommandNotifyReadStartDelay* const readStartDelay;
		CommandNotifyMinDuration* const minDuration;
		CommandNotifyMaxDuration* const maxDuration;

		explicit CommandTabNotifyTiming()
			: CommandList(nullptr, LIT("Notify Timing"), CMDNAMES())
			, readSpeed(createChild<CommandNotifyReadSpeed>())
			, readStartDelay(createChild<CommandNotifyReadStartDelay>())
			, minDuration(createChild<CommandNotifyMinDuration>())
			, maxDuration(createChild<CommandNotifyMaxDuration>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabNotifyTiming& GetCommandTabNotifyTiming();
}
