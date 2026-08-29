#include "Commands/Self/CommandNoBeast.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Game/script_thread.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	CommandNoBeast::CommandNoBeast(CommandList* const parent)
		: CommandToggle(parent, LOC("NOBEAST"), combineCommandNames(CMDNAMES("no", "block", "disable"), CMDNAMES_OBF("beast")), LOC("NOBEAST_H"))
	{
	}

	void CommandNoBeast::onChange(Click& click)
	{
		onChangeToggleScriptTickEventHandler(click, []
		{
			if (auto htb = GtaThread::fromHash(ATSTRINGHASH("am_hunt_the_beast")))
			{
				if (*ScriptLocal(htb, LOCAL_HTB_BEAST).as<int*>() == g_player)
				{
					Util::toast(LANG_FMT("NOBEAST_T", htb->getHost().getName()));
					htb->m_context.m_state = rage::scrThread::ABORTED;
				}
			}
		});
	}
}
