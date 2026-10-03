#pragma once
#include "Game/vector.hpp"
#include <cstdint>

enum eGpsNodePosInfo : uint32_t
{
    GNI_IGNORE_FOR_NAV   = 0x00008000,
    GNI_PLAYER_TRAIL_POS = 0x00004000,
};

class CGpsSlot
{
public:
    rage::vector4*  m_NodeCoordinates;
    void*           m_NodeAddress;
    uint16_t*       m_NodeInfo;
    int16_t*        m_NodeDistanceToTarget;
    rage::vector3   m_Destination;
    rage::vector3   m_PartialDestination;
    int32_t         m_iScriptIdSetGpsFlags;
    int32_t         m_NumNodes;
};
