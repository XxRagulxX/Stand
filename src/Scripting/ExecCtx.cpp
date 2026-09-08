#include "Scripting/ExecCtx.hpp"

#include <soup/os.hpp>

#include "Scripting/FiberPool.hpp"
#include "Scripting/Script.hpp"

namespace Stand
{
	thread_local ExecCtx g_ExecCtx;

	ExecCtx& ExecCtx::get() noexcept
	{
		return g_ExecCtx;
	}

	bool ExecCtx::isScript() const noexcept
	{
		return thread_context_is_script(tc);
	}

	bool ExecCtx::hasGameTls() const noexcept
	{
		return thread_context_has_game_tls(tc);
	}

	void ExecCtx::ensureScript(std::function<void()>&& f)
	{
		if (isScript())
		{
			f();
			return;
		}

		FiberPool::queueJob(std::move(f));
	}

	void ExecCtx::yield()
	{
		switch (tc)
		{
		case TC_SCRIPT_YIELDABLE:
			Script::current()->yield();
			break;

		case TC_OTHER:
			soup::os::sleep(10);
			break;

		default:
			SOUP_ASSERT_UNREACHABLE;
		}
	}
}