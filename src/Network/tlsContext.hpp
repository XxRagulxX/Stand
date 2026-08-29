#pragma once

#include "Util/struct_base.hpp"

#include <windows.h>

#pragma pack(push, 1)
namespace rage
{
	class tlsContext
	{
	public:
		PAD(0, 0x22AC) int thisThreadType; // CViewportManager pattern in main
		/* 0x22B8 */ sysMemAllocator* sysMemAllocator_sm_Current;
		/* 0x22C0 */ sysMemAllocator* sysMemAllocator_sm_Master;
		/* 0x22C8 */ sysMemAllocator* sysMemAllocator_sm_Container;
		PAD(0x22C8 + 8, 0x2A50) scrThread* m_script_thread; // 74 2C E8 ? ? ? ? FF 4B 10 + 3 rip = rage::scrThread::GetActiveThread
		/* 0x2A58 */ bool m_is_script_thread_active;

		[[nodiscard]] static tlsContext* get()
		{
			return *reinterpret_cast<tlsContext**>(__readgsqword(0x58));
		}
	};
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif
	//static_assert(sizeof(tlsContext) == 0x2A58 + sizeof(bool));
	static_assert(offsetof(tlsContext, m_script_thread) == 0x2A48); // 3889: 0x2A50 -> 0x2A48 (-0x8)

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
}
#pragma pack(pop)
