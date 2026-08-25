#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Config/conf.hpp"
#include "Core/main.hpp"
#include "Network/NetLoggingInterfaceShim.hpp"

namespace Stand
{
	class CommandShimnlObjectMgr : public CommandToggle
	{
	public:
		NetLoggingInterfaceShim shim{ "ObjectMgr" };
		rage::netLoggingInterface* og;

		explicit CommandShimnlObjectMgr(CommandList* const parent)
			: CommandToggle(parent, LIT("ObjectMgr"))
		{
		}

		void onEnable(Click& click) final
		{
			auto p = (rage::netLoggingInterface**)IDA_ADDR(00007FF7FDCD7A80);
			og = *p;
			*p = &shim;
		}

		void onDisable(Click& click) final
		{
			*(rage::netLoggingInterface**)IDA_ADDR(00007FF7FDCD7A80) = og;
		}
	};
}
