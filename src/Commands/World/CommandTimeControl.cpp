#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	static CommandSliderLegacy _NetworkTimeHour{"networktimehour", "Hour", "Set hour (0-23)", 0, 23, 12};
	static CommandSliderLegacy _NetworkTimeMinute{"networktimeminute", "Minute", "Set minute (0-59)", 0, 59, 0};
	static CommandSliderLegacy _NetworkTimeSecond{"networktimesecond", "Second", "Set second (0-59)", 0, 59, 0};


	class SetNetworkTime : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(_NetworkTimeHour.GetState(),
			    _NetworkTimeMinute.GetState(),
			    _NetworkTimeSecond.GetState());
		}
	};

	class FreezeNetworkTime : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(_NetworkTimeHour.GetState(),
			    _NetworkTimeMinute.GetState(),
			    _NetworkTimeSecond.GetState());
		}

		virtual void OnDisable() override
		{
			NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
		}
	};

	static SetNetworkTime _SetTime{"setnetworktime", "Set Time", "Sets the current online time"};
	static FreezeNetworkTime _FreezeTime{"freezenetworktime", "Freeze Time", "Freezes the online clock at the selected time"};
}
