#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

#include "Commands/Context/CommandCtxHotkey.hpp"
#include "Menu/ContextMenu.hpp"

namespace Stand
{
	class CommandCtxHotkeyHoldMode : public CommandToggle
	{
	public:
		explicit CommandCtxHotkeyHoldMode(CommandList* parent)
			: CommandToggle(parent, LOC("HOTKEY_HOLD"), {}, NOLABEL, ((CommandCtxHotkey*)parent)->hotkey->isHoldMode())
		{
		}

		void onChange(Click& click) final
		{
			parent->goBackIfActive(click.thread_context);
			auto* cmd = ContextMenu::getTargetPhysical();
			((CommandCtxHotkey*)parent)->hotkey->setHoldMode(m_on);
			cmd->updateHotkeysState();
			cmd->processVisualUpdate();
			cmd->onHotkeysChanged(click.type);
			cmd->updateHotkeysInContextMenu();
			((CommandCtxHotkey*)parent)->save(cmd);
		}
	};
}
