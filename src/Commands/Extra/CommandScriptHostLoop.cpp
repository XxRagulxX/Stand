#if false
#include "Commands/Extra/CommandScriptHostLoop.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Network/is_session.hpp"
#include "Scripting/ScriptHostUtil.hpp"

namespace Stand
{
	CommandScriptHostLoop::CommandScriptHostLoop(CommandList* parent)
		: CommandToggleRegular(parent, LOC("BESCRHSTKLEPTO"), CMDNAMES("klepto"), LOC("BESCRHSTKLEPTO_H"))
	{
	}

	void CommandScriptHostLoop::onEnableInner(Click& click)
	{
		registerScriptTickEventHandler(click, [this]
		{
			if (m_on)
			{
				if (is_session_started() && !g_player.isScriptHost() && ScriptHostUtil::canSafelyTakeIt())
				{
					ScriptHostUtil::takeIt();
				}
				return true;
			}
			return false;
		});
	}
}
#endif
