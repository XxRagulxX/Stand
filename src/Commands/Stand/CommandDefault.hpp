#pragma once

#include "Rendering/StandPort/CommandAction.hpp"
#include "Commands/Widgets/CommandList.hpp"

#include "Scripting/ExecCtx.hpp"
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
				ExecCtx::get().tc = TC_SCRIPT_NOYIELD;
				g_gui.root_list->recursivelyApplyDefaultState();
			});
		}
	};
}
