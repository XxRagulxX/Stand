#include "Scripting/ScriptMgr.hpp"

#include <thread>

#include "Scripting/FiberPool.hpp"
#include "Core/ExceptionHandler.hpp"

namespace Stand
{
	void ScriptMgr::addScript(HMODULE module, std::unique_ptr<Script>&& script)
	{
		m_Scripts[module].emplace_back(std::move(script));
	}

	HMODULE ScriptMgr::getScriptModule(const Script* script) const
	{
		for (const auto& [module, scripts] : m_Scripts)
		{
			for (const auto& scr : scripts)
			{
				if (scr.get() == script)
				{
					return module;
				}
			}
		}

		return nullptr;
	}

	void ScriptMgr::removeScripts(HMODULE module)
	{
		auto it = m_Scripts.find(module);

		if (it == m_Scripts.end())
			return;

		for (auto& script : it->second)
		{
			if (script)
			{
				script->stop();
			}
		}
	}

	void ScriptMgr::removeScript(script_func_t function)
	{
		for (auto& [module, scripts] : m_Scripts)
		{
			for (auto& script : scripts)
			{
				if (script && script->func == function)
				{
					script->stop();
					return;
				}
			}
		}
	}

	size_t ScriptMgr::getNumScripts(HMODULE module) const
	{
		auto it = m_Scripts.find(module);

		if (it == m_Scripts.end())
			return 0;

		return it->second.size();
	}

	void ScriptMgr::tick()
	{
		m_Ticking.store(true, std::memory_order_release);

		if (!IsThreadAFiber())
		{
			ConvertThreadToFiber(nullptr);
		}

		FiberPool::onTick();

		Script::ret_fiber = GetCurrentFiber();

		for (auto moduleIt = m_Scripts.begin(); moduleIt != m_Scripts.end();)
		{
			auto& scripts = moduleIt->second;

			for (auto scriptIt = scripts.begin(); scriptIt != scripts.end();)
			{
				bool ok = false;

				__try
				{
					ok = (*scriptIt)->tick();
				}
				__EXCEPTIONAL()
				{
				}

				if (ok)
				{
					++scriptIt;
				}
				else
				{
					scriptIt = scripts.erase(scriptIt);
				}
			}

			if (scripts.empty())
			{
				moduleIt = m_Scripts.erase(moduleIt);
			}
			else
			{
				++moduleIt;
			}
		}

		m_Ticking.store(false, std::memory_order_release);
	}

	void ScriptMgr::deinit()
	{

		while (m_Ticking.load(std::memory_order_acquire))
		{
			std::this_thread::yield();
		}

		for (auto& [module, scripts] : m_Scripts)
		{
			for (auto& script : scripts)
			{
				if (script)
				{
					script->stop();
				}
			}
		}

		m_Scripts.clear();

		m_Ticking.store(false, std::memory_order_release);
	}

}