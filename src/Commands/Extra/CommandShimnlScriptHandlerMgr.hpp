#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Network/NetLoggingInterfaceShim.hpp"
#include "Game/pointers.hpp"
#include "Game/scriptHandlerMgr.hpp"

namespace Stand
{
	class CommandShimnlScriptHandlerMgr : public CommandToggle
	{
	public:
		NetLoggingInterfaceShim shim{ "scriptHandlerMgr" };

		explicit CommandShimnlScriptHandlerMgr(CommandList* const parent)
			: CommandToggle(parent, LIT("scriptHandlerMgr"))
		{
		}

		void onEnable(Click& click) final
		{
			(*pointers::script_handler_mgr)->m_logger = &shim;
		}

		void onDisable(Click& click) final
		{
			(*pointers::script_handler_mgr)->m_logger = nullptr;
		}
	};
}
