#pragma once

#include <functional>

namespace Stand
{
	class FiberPool
	{
	public:
		static void queueJob(std::function<void()>&& func);

		static void onTick();
	};
}