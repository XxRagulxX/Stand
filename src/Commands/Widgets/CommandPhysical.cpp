#include "Commands/Widgets/CommandPhysical.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandHotkeyDispatch.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Rendering/Gui.hpp"
#include "Scripting/FiberPool.hpp"
#include "Menu/Click.hpp"

#include <algorithm>

namespace Stand
{
	void CommandPhysical::onTick()
	{
		if (m_tickHandler && !m_tickHandler())
		{
			m_tickHandler = nullptr;
			CommandTickDispatch::RemoveCommand(this);
		}
	}

	std::string CommandPhysical::getCommandSyntax() const
	{
		if (command_names.empty())
			return {};

		return "Command: " + command_names.front();
	}

	bool CommandPhysical::canBeUsedByOtherPlayers() const
	{
		return perm != COMMANDPERM_USERONLY && !command_names.empty();
	}

	void CommandPhysical::queueJob(std::function<void()>&& func)
	{
		if (!m_JobQueued)
		{
			m_JobQueued = true;
			Stand::FiberPool::queueJob([this, func{std::move(func)}] {
				m_JobQueued = false;
				func();
			});
		}
	}

	void CommandPhysical::queueJob(std::function<void(ThreadContext)>&& func)
	{
		if (!m_JobQueued)
		{
			m_JobQueued = true;
			Stand::FiberPool::queueJob([this, func{std::move(func)}] {
				m_JobQueued = false;
				func(TC_SCRIPT_YIELDABLE);
			});
		}
	}

	void CommandPhysical::ensureYieldableScriptThread(ThreadContext thread_context, std::function<void()>&& func)
	{
		if (thread_context == TC_SCRIPT_YIELDABLE)
			func();
		else
			queueJob(std::move(func));
	}

	void CommandPhysical::ensureYieldableScriptThread(const Click& click, std::function<void()>&& func)
	{
		ensureYieldableScriptThread(click.thread_context, std::move(func));
	}

	void CommandPhysical::ensureYieldableScriptThread(std::function<void()>&& func)
	{
		ensureYieldableScriptThread(TC_OTHER, std::move(func));
	}

	void CommandPhysical::ensureScriptThread(ThreadContext thread_context, std::function<void()>&& func)
	{
		if (thread_context_is_script(thread_context))
			func();
		else
			queueJob(std::move(func));
	}

	void CommandPhysical::ensureScriptThread(std::function<void()>&& func)
	{
		ensureScriptThread(TC_OTHER, std::move(func));
	}

	void CommandPhysical::ensureScriptThread(const Click& click, std::function<void()>&& func)
	{
		ensureScriptThread(click.thread_context, std::move(func));
	}

	void CommandPhysical::ensureScriptThread(Click& click, std::function<void(Click&)>&& func)
	{
		if (!m_JobQueued)
		{
			m_JobQueued = true;
			click.ensureScriptThread([this, func{std::move(func)}](Click& click) {
				m_JobQueued = false;
				func(click);
			});
		}
	}

	void CommandPhysical::queueWorkerJob(std::function<void()>&& func)
	{
		if (!m_JobQueued)
		{
			m_JobQueued = true;
			Stand::FiberPool::queueJob([this, func{std::move(func)}] {
				m_JobQueued = false;
				func();
			});
		}
	}

	void CommandPhysical::ensureWorkerContext(ThreadContext thread_context, std::function<void()>&& func)
	{
		if (thread_context == TC_WORKER)
			func();
		else
			queueWorkerJob(std::move(func));
	}

	void CommandPhysical::ensureWorkerContext(const Click& click, std::function<void()>&& func)
	{
		ensureWorkerContext(click.thread_context, std::move(func));
	}

	bool CommandPhysical::canHotkeyBeRemoved(const Hotkey hotkey) const noexcept
	{
		for (const auto& h : m_DefaultHotkeys)
			if (h == hotkey) return false;
		return true;
	}

	bool CommandPhysical::canCountAsCommandWithHotkeys() const noexcept
	{
		return supportsStateOperations();
	}

	void CommandPhysical::removeHotkey(const Hotkey hotkey)
	{
		auto it = std::find(hotkeys.begin(), hotkeys.end(), hotkey);
		if (it != hotkeys.end())
			hotkeys.erase(it);
		updateHotkeysState();
	}

	void CommandPhysical::updateHotkeysState()
	{
		if (hotkeys.empty())
			CommandHotkeyDispatch::RemoveCommand(this);
		else
			CommandHotkeyDispatch::AddCommand(this);
	}

	void CommandPhysical::removeFromCommandsWithHotkeys()
	{
		CommandHotkeyDispatch::RemoveCommand(this);
	}

	void CommandPhysical::onHotkeysChanged(ClickType type)
	{
	}

	void CommandPhysical::processVisualUpdate() const
	{
	}

	void CommandPhysical::loadState(ClickType type)
	{
		auto it = g_gui.active_profile.data.find(getPathConfig());
		if (it != g_gui.active_profile.data.end())
		{
			Click click(type, TC_SCRIPT_NOYIELD);
			setState(click, it->second);
		}
		else
		{
			applyDefaultState();
		}
	}

	void CommandPhysical::updateHotkeysInContextMenu()
	{
	}

	std::string CommandPhysical::getActivationName() const
	{
		return menu_name.getLocalisedUtf8();
	}

	CommandPhysical* CommandPhysical::getStateCommand()
	{
		if (supportsStateOperations())
			return this;
		CommandPhysical* node = this->parent;
		while (node != nullptr)
		{
			if (node->supportsStateOperations())
				return node;
			node = node->parent;
		}
		return nullptr;
	}
}
