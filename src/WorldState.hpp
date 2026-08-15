#pragma once

#include <windows.h>

namespace Stand
{
	struct WorldState
	{
		[[nodiscard]] static BOOL getOnline();
		static void setOnline(BOOL toggle);
	};
}
