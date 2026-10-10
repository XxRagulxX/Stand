#include "Commands/Context/CommandCtxHotkey.hpp"

#include "Commands/Context/CommandCtxHotkeyHoldMode.hpp"
#include "Commands/Context/CommandCtxHotkeyRemove.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandCtxHotkey::CommandCtxHotkey(CommandList* parent, Hotkey* hotkey, CommandPhysical* target)
		: CommandList(parent, LIT(hotkey->toString())), hotkey(hotkey)
	{
		if (target->canHotkeyBeRemoved(*hotkey))
		{
			this->createChild<CommandCtxHotkeyRemove>(target);
		}
		else
		{
			this->createChild<CommandLambdaAction>(LOC("HOTKEY_C2"), CMDNAMES(), NOLABEL, [this](Click& click)
			{
				this->goBackIfActive(click.thread_context);
				g_gui.changeHotkeyOnFocusedCommand();
			});
		}
		if (target->isToggle())
		{
			this->createChild<CommandCtxHotkeyHoldMode>();
		}
	}

	void CommandCtxHotkey::save(CommandPhysical* target)
	{
		g_gui.hotkeys.save();
	}
}
