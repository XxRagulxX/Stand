#pragma once

#include "Game/AbstractEntity.hpp"

namespace Stand
{
	struct GhostDriver
	{
		inline static bool user_is_rcing = false;

		AbstractEntity driver{};

		[[nodiscard]] bool initVehicle(AbstractEntity& veh) { return false; }
		void initDriver(AbstractEntity& outDriver) {}
		[[nodiscard]] bool needsCleanup() const noexcept { return false; }
		void cleanup() {}
	};
}
