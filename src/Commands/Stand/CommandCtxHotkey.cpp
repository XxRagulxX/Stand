#include "Commands/Stand/CommandCtxHotkey.hpp"

#include "Commands/Stand/CommandCtxHotkeyHoldMode.hpp"
#include "Commands/Stand/CommandCtxHotkeyRemove.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Rendering/Gui.hpp"
#include "Util/Util.hpp"

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
			this->createChild<CommandLambdaAction>(LOC("HOTKEY_C2"), {}, NOLABEL, [this](Click& click)
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
		Util::toast(LANG_FMT("HOTKEY_T", target->getActivationName().getLocalisedUtf8()), TOAST_ABOVE_MAP);
	}
}
