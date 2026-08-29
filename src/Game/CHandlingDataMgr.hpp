#pragma once

#include "Game/atArray.hpp"
#include "Game/CHandlingData.hpp"

class CHandlingDataMgr
{
public:
	rage::atArray<CHandlingData*> m_HandlingData;

	[[nodiscard]] CHandlingData* getHandlingDataByNameHash(hash_t hash)
	{
		for (const auto& hd : m_HandlingData)
		{
			if (hd->m_handlingName == hash)
			{
				return hd;
			}
		}
		return nullptr;
	}
};
