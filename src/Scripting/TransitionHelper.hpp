#pragma once

#include <vector>
#include <cstdint>

#include "Scripting/SessionType.hpp"

namespace Stand
{
	struct TransitionHelper
	{
		inline static bool fix_stucks = true;

		[[nodiscard]] static bool isPrologueDone();
		static void skipPrologue();

		static bool doTransition(SessionType session_type, bool end_when_almost_done = false);
		static void abortTransition();
		static void forceQuit();
	};
}
