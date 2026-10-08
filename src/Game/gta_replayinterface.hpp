#pragma once

#include <cstdint>

#include "Game/gta_fwddecl.hpp"

template<class T>
class CEntityTracker
{
public:
	T* ptr;
	int16_t m_iInstID;
};
static_assert(sizeof(CEntityTracker<CVehicle>) == 16);

template<class T>
class CEntityRegistry
{
private:
	uint64_t pad[5];
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
	char pad_0x00[0x10 - 0x00];
public:
	class CReplayInterface<CVehicle, 0x158>* veh_interface;
	class CReplayInterface<CPed, 0xD8>* ped_interface;
	class CReplayInterface<CPickup, 0xD8>* pickup_interface;
	class CReplayInterface<CObject, 0x130>* object_interface;
};
