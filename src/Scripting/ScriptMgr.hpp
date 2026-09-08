#pragma once

#include <Windows.h>

#include <atomic>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Scripting/Script.hpp"

namespace Stand
{
	class ScriptMgr
	{
	public:
		ScriptMgr() = default;

		~ScriptMgr() = default;

		ScriptMgr(const ScriptMgr&) = delete;

		ScriptMgr& operator=(const ScriptMgr&) = delete;

		ScriptMgr(ScriptMgr&&) = delete;

		ScriptMgr& operator=(ScriptMgr&&) = delete;


		void addScript(HMODULE module, std::unique_ptr<Script>&& script);

		[[nodiscard]] HMODULE getScriptModule(const Script* script) const;

		void removeScripts(HMODULE module);

		void removeScript(script_func_t function);

		[[nodiscard]] std::size_t getNumScripts(HMODULE module) const;

		void tick();

		void deinit();

		[[nodiscard]] bool isTicking() const noexcept
		{
			return m_Ticking.load(std::memory_order_acquire);
		}

	private:
		std::unordered_map<HMODULE, std::vector<std::unique_ptr<Script>>> m_Scripts;

		std::atomic_bool m_Ticking{false};

	};

	inline ScriptMgr g_script_mgr;
}