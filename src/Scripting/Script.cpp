#include "Scripting/Script.hpp"

#include <soup/base.hpp>

#include "Scripting/ExecCtx.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Util/Util.hpp"
#include "Util/get_current_time_millis.hpp"

namespace Stand
{
	Script::Script(script_func_t func) :
	    func(func)
	{
		fiber = CreateFiber(0, [](void* param) {static_cast<Script*>(param)->fiberFunc();},this);
	}

	Script::~Script()
	{
		stop();
	}

	Script* Script::current()
	{
		return static_cast<Script*>(GetFiberData());
	}

	HANDLE Script::getModule() const
	{
		return g_script_mgr.getScriptModule(this);
	}

	bool Script::isCurrent() const noexcept
	{
		return GetCurrentFiber() == fiber;
	}

	void Script::stop()
	{
		if (fiber == nullptr)
		{
			return;
		}

		DeleteFiber(fiber);
		fiber = nullptr;
	}

	bool Script::tick()
	{
		if (fiber == nullptr)
		{
			return false;
		}

		ExecCtx::get().tc = TC_SCRIPT_YIELDABLE;

		SwitchToFiber(fiber);

		return func != nullptr;
	}

	void Script::nestedTick()
	{
		SOUP_ASSERT(ExecCtx::get().isScript());
		SOUP_ASSERT(!isCurrent());

		const auto previous_ret_fiber = ret_fiber;

		ret_fiber = GetCurrentFiber();

		tick();

		ret_fiber = previous_ret_fiber;
	}

	void Script::yield()
	{
		SOUP_ASSERT(ret_fiber != nullptr);

		SwitchToFiber(ret_fiber);
	}

	void Script::yield(time_t minSleepMs)
	{
		const auto deadline = get_current_time_millis() + minSleepMs;

		do
		{
			yield();
		} while (!IS_DEADLINE_REACHED(deadline));
	}

	void Script::fiberFunc()
	{
		__try
		{
			func();
		}
		__EXCEPTIONAL()
		{
		}

		func = nullptr;

		for (;;)
		{
			yield();
		}
	}
}