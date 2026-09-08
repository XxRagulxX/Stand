#pragma once
#include <AsyncLogger/Logger.hpp>
#include <MinHook.h>
#include <windows.h>
#include <atomic>

#include "Core/ImTypes.hpp"


#include <nlohmann/json.hpp>

using namespace al;

#undef Yield

namespace Stand
{
	using namespace std::chrono_literals;
	using namespace std::string_view_literals;

	extern std::atomic<bool> g_Running;
	extern HINSTANCE g_DllInstance;
	extern HANDLE g_MainThread;
	extern DWORD g_MainThreadId;
}