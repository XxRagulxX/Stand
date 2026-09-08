#include "Commands/Widgets/CommandTickDispatch.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Scripting/ScriptMgr.hpp"

#include <algorithm>

namespace Stand
{
	void CommandTickDispatch::AddCommandImpl(CommandPhysical* command)
	{
		// Defensive dedup - every real caller today only ever adds from
		// its own onEnable() (guarded upstream by CommandToggleNoCorrelation
		// only calling onEnable() on a genuine off->on transition - see
		// that file's own onClick()/setStateBool()), but a duplicate
		// entry here would tick the same command twice a frame instead
		// of crashing, so it's cheap enough to just guard against outright.
		if (command && std::ranges::find(m_Commands, command) == m_Commands.end())
			m_Commands.push_back(command);
	}

	void CommandTickDispatch::RemoveCommandImpl(CommandPhysical* command)
	{
		std::erase(m_Commands, command);
	}

	void CommandTickDispatch::RunScript()
	{
		GetInstance().RunScriptImpl();
	}

	void CommandTickDispatch::RunScriptImpl()
	{
		while (Stand::g_Running)
		{
			// Copied rather than iterated in place - a command's own
			// onTick() could plausibly call RemoveCommand() on some other
			// registered command (nothing does today, but nothing rules
			// it out either), which would otherwise invalidate this
			// loop's own iterator mid-pass.
			auto commands = m_Commands;
			for (auto* command : commands)
				command->onTick();

			Stand::Script::current()->yield();
		}
	}
}
