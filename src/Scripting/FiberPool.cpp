#include "Scripting/FiberPool.hpp"

#include <soup/AtomicDeque.hpp>

#include "Scripting/Script.hpp"
#include "Scripting/ScriptMgr.hpp"

namespace Stand
{
	static soup::AtomicDeque<std::function<void()>> g_JobQueue;

	void FiberPool::queueJob(std::function<void()>&& func)
	{
		g_JobQueue.emplace_front(std::move(func));
	}

	void FiberPool::onTick()
	{
		auto jobs = g_JobQueue.size();

		while (jobs--)
		{
			g_script_mgr.addScript(
			    GetModuleHandle(nullptr),
			    std::make_unique<Script>([] {
				    auto job = g_JobQueue.pop_back();

				    if (job)
					    (*job)();
			    }));
		}
	}
}