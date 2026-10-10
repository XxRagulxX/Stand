#include "Commands/Context/CommandCtxHotkeys.hpp"

#include "Commands/Context/CommandCtxHotkey.hpp"
#include "Rendering/StandPort/CommandDivider.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Menu/ContextMenu.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandCtxHotkeys::CommandCtxHotkeys(CommandList* const parent)
		: CommandList(parent, LOC("HOTKEYS"))
	{
		populate();
	}

	void CommandCtxHotkeys::onBack(ThreadContext thread_context)
	{
		if (close_context_menu_on_back)
		{
			ensureYieldableScriptThread(thread_context, []
			{
				ContextMenu::close(TC_SCRIPT_YIELDABLE);
			});
		}
	}

	void CommandCtxHotkeys::populate()
	{
		CommandPhysical* const target = ContextMenu::getTargetPhysical();

		this->createChild<CommandLambdaAction>(LOC("HOTKEY_A"), CMDNAMES(), NOLABEL, [this](Click&)
		{
			g_gui.addHotkeyToFocusedCommand();
		});
		if (!target->hotkeys.empty())
		{
			this->createChild<CommandDivider>(LOC("HOTKEYS"));
			for (auto& hotkey : target->hotkeys)
			{
				this->createChild<CommandCtxHotkey>(&hotkey, target);
			}
		}
	}

	void CommandCtxHotkeys::update()
	{
		resetChildren();
		populate();
		processChildrenUpdate();
	}
}
