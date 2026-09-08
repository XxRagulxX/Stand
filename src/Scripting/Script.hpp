#pragma once

#include <Windows.h>

#include "Game/typedecl.hpp"

namespace Stand
{
	using script_func_t = void (*)();

	class Script
	{
	public:
		inline static void* ret_fiber = nullptr;

		script_func_t func;

	private:
		void* fiber = nullptr;

	public:
		explicit Script(script_func_t func);

		~Script();

		[[nodiscard]] static Script* current();

		[[nodiscard]] HANDLE getModule() const;

		[[nodiscard]] bool isCurrent() const noexcept;

		void stop();

		bool tick();

		void nestedTick();

		void yield();

		void yield(time_t minSleepMs);

	private:
		void fiberFunc();
	};
}