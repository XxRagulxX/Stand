#pragma once

#include <cstdint>

#include "Game/gta_player.hpp"

class CEntity;

#pragma pack(push, 1)
namespace rage
{
	class netObject
	{
	public:
		virtual ~netObject() = default;
		virtual CEntity* GetEntity() { return nullptr; }

		static const unsigned GLOBALFLAG_PERSISTENTOWNER = 1u << 0;
		static const unsigned GLOBALFLAG_CLONEALWAYS = 1u << 1;

		char _pad_08[0x0A - 0x08];
		uint16_t object_id;
		char _pad_0C[0x49 - 0x0C];
		compactplayer_t owner_id;
		compactplayer_t next_owner_id;
		bool is_clone;
		char _pad_4C[0x4E - 0x4C];
		uint16_t global_flags;
	};
}

class CNetObjPhysical : public rage::netObject
{
public:
	char _pad_netobj[0x100 - sizeof(rage::netObject)];
	bool m_isInWater;
};

class CNetObjPed : public CNetObjPhysical
{
public:
	char _pad_netphys[0x200 - sizeof(CNetObjPhysical)];
};

class CNetObjPlayer : public CNetObjPed
{
public:
	char _pad_netped[0x4A0 - sizeof(CNetObjPed)];
	bool m_bIsPassive : 1;
};
#pragma pack(pop)

class CNetworkObjectMgr
{
public:
	rage::netObject* find_object_by_id(uint16_t id, bool) { return nullptr; }
	void unregisterNetworkObject(rage::netObject* obj, int, bool, bool) {}
};
