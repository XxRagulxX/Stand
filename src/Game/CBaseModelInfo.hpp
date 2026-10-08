#pragma once

#include <cstdint>

#include "Util/hashtype.hpp"

enum ModelInfoType : uint8_t
{
	MI_TYPE_NONE,
	MI_TYPE_BASE,
	MI_TYPE_MLO,
	MI_TYPE_TIME,
	MI_TYPE_WEAPON,
	MI_TYPE_VEHICLE,
	MI_TYPE_PED,
	MI_TYPE_COMPOSITE,
};

class CBaseModelInfo
{
public:
	virtual ~CBaseModelInfo() = default;
	char _pad_08[0x10];
	hash_t hash;
	char _pad_1C[0x50 - 0x1C];
	uint32_t flags;
	char _pad_54[0x9D - 0x54];
	uint8_t m_type;
	char _pad_9E[0xB0 - 0x9E];

	[[nodiscard]] uint8_t GetModelType() const noexcept
	{
		return m_type;
	}
};
static_assert(sizeof(CBaseModelInfo) == 0xB0);
