#pragma once

#include <windows.h>

#include "AntiCheat/ScriptFuncPatch.hpp"

namespace Stand
{
	class DisableDripfeed
	{
	public:
		inline static BOOL enabled = true;

	private:
		inline static ScriptFuncPatch patch;

	public:
		static void onTick();
		static void onDisable();
	};
}
