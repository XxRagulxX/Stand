#include "Commands/Widgets/CommandPhysical.hpp"

#include "Scripting/FiberPool.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	std::string CommandPhysical::getCommandSyntax() const
	{
		// Real Stand's own CommandPhysical::getCommandSyntax() (Commands/
		// Widgets/CommandPhysical.cpp on origin/stand-reference) returns
		// std::wstring through its own LANG_GET_W("CMD") lookup - this
		// project has no wide-string/localisation pipeline for any of
		// its own ported Label text (see Util/Label.hpp's own class
		// comment), so this is plain UTF-8 with the literal "Command"
		// prefix, matching every other LIT()-based text in this port.
		if (command_names.empty())
			return {};

		return "CommandLegacy: " + command_names.front();
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
}
