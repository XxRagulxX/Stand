#pragma once
#include "Commands/Widgets/CommandHotkeyDispatch.hpp"
#include "Commands/Widgets/CommandIssuable.hpp"
#include "Commands/Widgets/CommandStateSerializer.hpp"
#include "Menu/Hotkey.hpp"
#include "Util/Label.hpp"

#include <functional>
#include <vector>

// Real Stand's CommandPhysical also carries a web-command viewport
// (isInViewport/onTickInWebViewport/isActiveOnWeb/updateWebState), a
// right-click context-menu integration (isOpenInContextMenu/
// updateHotkeysInContextMenu), its own per-command hotkey-binding UI
// (applyDefaultHotkeys/modifyHotkeys/hasHotkey/onHotkeysChanged - a
// different model from this project's own existing, chain-based
// HotkeySystem), commercial-edition feature-list text, chat-command
// syntax/argument helpers, and a generic tick-event-handler registration
// system tied to its own TickMgr - none of which this project has or is
// building. Kept the part every widget actually needs: display content
// (menu_name/help_text/hotkeys, the hotkeys field kept only as a data
// member for now - not wired to any input dispatch yet), the getState/
// setState/applyDefaultState virtual triplet a real command overrides
// with its actual behaviour, onClick/onLeft/onRight dispatch, and the
// ensureScriptThread/ensureYieldableScriptThread/ensureWorkerContext/
// queueJob family (same shape as Stand's own, through this project's own
// FiberPool).
namespace Stand
{
	class CommandPhysical : public CommandIssuable
	{
	private:
		bool m_JobQueued = false;

	public:
		Label menu_name;
		Label help_text;
		std::vector<Hotkey> hotkeys;

		explicit CommandPhysical(CommandType type, CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, commandflags_t flags = 0, CommandPerm perm = COMMANDPERM_USERONLY, const std::vector<Hotkey>& default_hotkeys = {}) :
		    CommandIssuable(parent, std::move(command_names), perm, flags, type),
		    menu_name(std::move(menu_name)),
		    help_text(std::move(help_text)),
		    hotkeys(default_hotkeys)
		{
			// See CommandStateSerializer.hpp's own class comment - only
			// commands whose state is actually meant to persist register
			// here at all.
			if (supportsStateOperations())
				CommandStateSerializer::AddCommand(this);

			// See CommandHotkeyDispatch.hpp's own class comment - only
			// commands actually constructed with a hotkey register here.
			if (!this->hotkeys.empty())
				CommandHotkeyDispatch::AddCommand(this);
		}

		~CommandPhysical() override
		{
			CommandStateSerializer::RemoveCommand(this);
			CommandHotkeyDispatch::RemoveCommand(this);
		}

		[[nodiscard]] const Label& getMenuName() const
		{
			return menu_name;
		}

		void setMenuName(Label&& menu_name)
		{
			this->menu_name = std::move(menu_name);
		}

		void setHelpText(const Label& help_text)
		{
			this->help_text = help_text;
		}

		void setHelpText(Label&& help_text)
		{
			this->help_text = std::move(help_text);
		}

		[[nodiscard]] bool supportsStateOperations() const noexcept
		{
			return (flags & CMDFLAG_SUPPORTS_STATE_OPERATIONS) != 0;
		}

		[[nodiscard]] bool supportsSavedState() const noexcept
		{
			return (flags & CMDFLAG_NO_SAVED_STATE) == 0;
		}

		// Ported from real Stand's own CommandPhysical::getCommandSyntax()
		// (CommandPhysical.cpp on origin/stand-reference) - "Command: "
		// plus the command's own primary name (command_names.front()),
		// e.g. "Command: godmode". CommandToggleNoCorrelation overrides
		// this to append " [on/off]" (its own real Stand equivalent,
		// confirmed against origin/stand-reference's own
		// CommandToggleNoCorrelation::getCommandSyntax()) - see
		// GridItemStandCommand::GetDescription() for where this actually
		// shows up (the second line under a focused command's own
		// help_text, matching real Stand's own populateCorner()). Not
		// gated behind anything like real Stand's own g_gui.show_syntax
		// setting - this project has no such setting, so it's always
		// shown whenever it's non-empty.
		[[nodiscard]] virtual std::string getCommandSyntax() const;

		virtual void onClick(Click& click)
		{
		}

		virtual bool onLeft(Click& click, bool holding)
		{
			return true;
		}

		virtual bool onRight(Click& click, bool holding)
		{
			return true;
		}

		[[nodiscard]] virtual std::string getState() const
		{
			return {};
		}

		[[nodiscard]] virtual std::string getDefaultState() const
		{
			return {};
		}

		virtual void setState(Click& click, const std::string& state)
		{
		}

		// Needs a script thread; NOYIELD is fine - same requirement real
		// Stand's own applyDefaultState() has.
		virtual void applyDefaultState()
		{
		}

		// NOT real Stand's own generic tick-event-handler system (tied
		// to its own TickMgr - see this class's own top comment, not
		// ported) - a plain empty-default virtual instead, called only
		// for whichever commands opt into CommandTickDispatch themselves
		// (see that class's own doc comment for why this isn't wired in
		// automatically here the way getState()/setState()/
		// applyDefaultState() are for CommandStateSerializer above).
		virtual void onTick()
		{
		}

		void queueJob(std::function<void()>&& func);
		void queueJob(std::function<void(ThreadContext)>&& func);

		void ensureYieldableScriptThread(ThreadContext thread_context, std::function<void()>&& func);
		void ensureYieldableScriptThread(const Click& click, std::function<void()>&& func);
		void ensureYieldableScriptThread(std::function<void()>&& func);

		void ensureScriptThread(ThreadContext thread_context, std::function<void()>&& func);
		void ensureScriptThread(std::function<void()>&& func);
		void ensureScriptThread(const Click& click, std::function<void()>&& func);
		void ensureScriptThread(Click& click, std::function<void(Click&)>&& func);

		void ensureWorkerContext(ThreadContext thread_context, std::function<void()>&& func);
		void ensureWorkerContext(const Click& click, std::function<void()>&& func);

	private:
		void queueWorkerJob(std::function<void()>&& func);
	};
}
