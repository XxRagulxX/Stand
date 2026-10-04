#pragma once
#include "Game/vector.hpp"
#include <cstdint>

enum eGpsNodePosInfo : uint32_t
{
    GNI_IGNORE_FOR_NAV   = 0x00008000,
    GNI_PLAYER_TRAIL_POS = 0x00004000,
};

// Enhanced layout: vtable inserted at +0x00, m_NodeAddress removed,
// keeping m_NumNodes at +0x44 (confirmed by `cmp [rax+44h],0` at call site).
class CGpsSlot
{
public:
    /* 0x00 */ void*           vtable;
    /* 0x08 */ rage::vector4*  m_NodeCoordinates;
    /* 0x10 */ uint16_t*       m_NodeInfo;
    /* 0x18 */ int16_t*        m_NodeDistanceToTarget;
    /* 0x20 */ rage::vector3   m_Destination;
    /* 0x2C */ float           _pad2C;
    /* 0x30 */ rage::vector3   m_PartialDestination;
    /* 0x3C */ float           _pad3C;
    /* 0x40 */ int32_t         m_iScriptIdSetGpsFlags;
    /* 0x44 */ int32_t         m_NumNodes;
};
