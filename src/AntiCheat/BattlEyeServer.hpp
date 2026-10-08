#pragma once

namespace BattlEyeServer
{
	[[nodiscard]] inline bool isRunning() { return false; }
	[[nodiscard]] inline bool isRunningDueToUs() { return false; }
	inline void start() {}
	inline void stop() {}
}
