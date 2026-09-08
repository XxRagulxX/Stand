#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Pointers.hpp"
#include "Core/ModuleMgr.hpp"
#include "Core/PointerCalculator.hpp"
#include "Util/Joaat.hpp"
#include "Core/Hooking.hpp"

#include <intrin.h>

namespace Stand::Hooks
{
	static bool IsAddressInGameMemory(void* addr)
	{
		auto as_int = reinterpret_cast<uintptr_t>(addr);
		static auto module = g_ModuleMgr.Get("GTA5_Enhanced.exe"_J); // this should always succeed

		return as_int >= module->Base() && as_int <= module->End();
	}

	static bool IsPotentialArxanCallback(void* addr)
	{
		if (!addr || !IsAddressInGameMemory(addr))
			return false;

		auto as_bytes = reinterpret_cast<std::uint8_t*>(addr);

		if (as_bytes[0] != 0xE9)
			return false;

		auto jmp_addr = PointerCalculator(addr).Add(1).Rip().As<void*>();

		if (!IsAddressInGameMemory(jmp_addr))
			return false;

		return true; // likely, yes
	}

	void Anticheat::QueueDependency(__int64 a1)
	{
		auto f1 = *reinterpret_cast<void**>(a1 + 0x60);
		auto f2 = *reinterpret_cast<void**>(a1 + 0x100);
		auto f3 = *reinterpret_cast<void**>(a1 + 0x1A0);

		if (!f1)
			return Hooking::Get<Anticheat::QueueDependency>()->Original<decltype(&Anticheat::QueueDependency)>()(a1);

		if (f1 == Pointers.SigScanMemory)
		{
			LOGF(WARNING, "QueueDependency: Blocked the sig scanner dependency from being queued");
			return;
		}

		int arx_score = (int)IsPotentialArxanCallback(f1) + (int)IsPotentialArxanCallback(f2) + (int)IsPotentialArxanCallback(f3);

		if (arx_score >= 2)
		{
			return;
		}

		Hooking::Get<Anticheat::QueueDependency>()->Original<decltype(&Anticheat::QueueDependency)>()(a1);
	}
}