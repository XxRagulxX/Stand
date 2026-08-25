#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Core/ExecCtx.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandDefault : public CommandAction
	{
	public:
		explicit CommandDefault(CommandList* const parent)
			: CommandAction(parent, LOC("GODFLT"), { CMDNAME("default"), CMDNAME("applydefaultstate") }, LOC("GODFLT_H"))
		{
		}

		void onClick(Click& click) final
		{
			click.setResponse(LOC("GODFLT_T"));
			ensureYieldableScriptThread(click, []
			{
				g_gui.disableAutosave();
				g_gui.cancelPendingSave();
				ExecCtx::get().tc = TC_SCRIPT_NOYIELD;
				g_gui.recursivelyApplyDefaultState();
			});
		}
	};
}
