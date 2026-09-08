#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"


namespace Stand::Hooks
{
	void Anticheat::HttpStartRequest(void* request)
	{
		uintptr_t req_ptr = reinterpret_cast<uintptr_t>(request);
		const char** host_ptr = reinterpret_cast<const char**>(req_ptr + 0x18F8);
		const char* path = *reinterpret_cast<const char**>(req_ptr + 0x1900);

		// LOG(VERBOSE) << *host_ptr << " " << path;

		if (path && strstr(path, "GameTransactions.asmx/Bonus"))
		{
			*const_cast<const char**>(host_ptr) = "0.0.0.0";
			LOG(WARNING) << "Blocked Bonus Report";
		}

		return Hooking::Get<Anticheat::HttpStartRequest>()->Original<decltype(&Anticheat::HttpStartRequest)>()(request);
	}
}