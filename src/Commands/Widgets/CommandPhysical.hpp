#pragma once
#include "Commands/Widgets/CommandHotkeyDispatch.hpp"
#include "Commands/Widgets/CommandIssuable.hpp"
#include "Commands/Widgets/CommandStateSerializer.hpp"
#include "Menu/Hotkey.hpp"
#include "Util/Label.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace Stand::Rendering { class GridItem; class Grid; }

namespace Stand
{
	class CommandPhysical : public CommandIssuable
	{
	private:
		bool m_JobQueued = false;

	protected:
		std::function<bool()> m_tickHandler;

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
			if (supportsStateOperations())
				CommandStateSerializer::AddCommand(this);

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

		[[nodiscard]] virtual std::string getCommandSyntax() const;
		[[nodiscard]] bool canBeUsedByOtherPlayers() const;

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

		virtual void applyDefaultState()
		{
		}

		virtual void onTick();
		virtual void onTickInGameViewport() {}
		virtual void onTickInWebViewport() {}
		virtual void onPreScriptedAccess() {}

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

		virtual void fillVirtualItems(std::vector<std::unique_ptr<Rendering::GridItem>>&, Rendering::Grid*) {}

	private:
		void queueWorkerJob(std::function<void()>&& func);
	};
}
