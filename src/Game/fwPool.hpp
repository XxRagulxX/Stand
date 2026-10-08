#pragma once

#include <cstdint>

namespace rage
{
#ifndef POOL_FLAG_ISFREE
#define POOL_FLAG_ISFREE 0x80
#endif
#ifndef POOL_FLAG_REFERENCEMASK
#define POOL_FLAG_REFERENCEMASK 0x7f
#endif

	struct fwBasePool
	{
		uint8_t* m_aStorage;
		uint8_t* m_aFlags;
		int32_t m_nSize;
		int32_t m_nStorageSize;
		int32_t m_nFirstFreeIndex;
		int32_t m_nLastFreeIndex;
		int32_t m_numSlotsUsed : 30;
		int32_t m_bOwnsArrays : 2;

		[[nodiscard]] void* GetAt(int32_t index) const noexcept
		{
			uint32_t i = (index >> 8);
			if (m_aFlags[i] == (index & 0xff))
			{
				return (void*)&m_aStorage[i * m_nStorageSize];
			}
			return nullptr;
		}

		[[nodiscard]] int32_t GetIndex(const void* ptr) const noexcept
		{
			const auto i = GetJustIndex(ptr);
			return (i << 8) + m_aFlags[i];
		}

		[[nodiscard]] int32_t GetJustIndex(const void* ptr) const noexcept
		{
			return (int32_t)((((const uint8_t*)ptr) - ((const uint8_t*)&m_aStorage[0])) / m_nStorageSize);
		}

		[[nodiscard]] bool GetIsFree(int32_t index) const noexcept
		{
			return (m_aFlags[index] & POOL_FLAG_ISFREE) != 0;
		}

		void SetReference(int32_t index, uint8_t nReference) noexcept
		{
			m_aFlags[index] = (m_aFlags[index] & ~POOL_FLAG_REFERENCEMASK) | (((nReference & POOL_FLAG_REFERENCEMASK) > 1 ? (nReference & POOL_FLAG_REFERENCEMASK) : 1));
		}
	};

	template <typename T>
	struct fwPool : public fwBasePool
	{
		[[nodiscard]] T* GetAt(int32_t index) const noexcept
		{
			return static_cast<T*>(fwBasePool::GetAt(index));
		}
	};
}
