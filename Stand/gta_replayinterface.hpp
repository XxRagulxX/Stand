#pragma once

#include <cstdint>

#include "gta_fwddecl.hpp"

template<class T>
class CEntityTracker
{
public:
	T* ptr; // may be nullptr
	int16_t m_iInstID;
};
static_assert(sizeof(CEntityTracker<CVehicle>) == 16);

template<class T>
class CEntityRegistry
{
private:
	uint64_t pad[5]; // sysCriticalSectionToken
public:
	class CEntityTracker<T>* entities;
	size_t m_count;
	size_t m_entityTrackersUsed;
};

template<class T, int32_t reg_offset>
class CReplayInterface
{
private:
	char pad_0[reg_offset];
public:
	class CEntityRegistry<T> m_entityRegistry;
};

class CReplayInterfaceGame
{
private:
	/* 0x00 */ char pad_0x00[0x10 - 0x00];
public:
	/* 0x10 */ class CReplayInterface<CVehicle, 0x158>* veh_interface;
	/* 0x18 */ class CReplayInterface<CPed, 0xD8>* ped_interface;
	/* 0x20 */ class CReplayInterface<CPickup, 0xD8>* pickup_interface;
	/* 0x28 */ class CReplayInterface<CObject, 0x130>* object_interface;
};
