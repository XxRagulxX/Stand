#include "Core/tbDisableDeathbarriers.hpp"

#include "Core/FiberPool.hpp"
#include "Core/regular_event.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	void tbDisableDeathbarriers::onEnable()
	{
		FiberPool::queueJob([this]
		{
			reScriptTickEvent::registerHandlerInThreadContext([this]
			{
				*ScriptGlobal(GLOBAL_DISABLE_DEATH_BARRIERS).as<int*>() = isEnabled();
				return isEnabled();
			}, "toggleable behaviour");
		});
	}
}
