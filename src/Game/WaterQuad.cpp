#include "Game/WaterQuad.hpp"
#include "Core/Pointers.hpp"

// Enhanced layout: Pointers.water_quads = WaterQuad*** pointing to a global.
// *water_quads  = struct ptr  (heap)
// **water_quads = WaterQuad*  (array, at struct+0)
// count         = uint16_t    (at struct+8)
static WaterQuad* getArray()
{
    if (!Stand::Pointers.water_quads) return nullptr;
    auto* mgr = reinterpret_cast<std::uint8_t*>(*Stand::Pointers.water_quads);
    if (!mgr) return nullptr;
    return *reinterpret_cast<WaterQuad**>(mgr);
}

static std::uint16_t getCount()
{
    if (!Stand::Pointers.water_quads) return 0;
    auto* mgr = reinterpret_cast<std::uint8_t*>(*Stand::Pointers.water_quads);
    if (!mgr) return 0;
    return *reinterpret_cast<std::uint16_t*>(mgr + 8);
}

WaterQuad* WaterQuad::at(std::int16_t x, std::int16_t y)
{
    WaterQuad* quads = getArray();
    if (!quads) return nullptr;
    const std::uint16_t count = getCount();
    for (std::uint16_t i = 0; i < count; ++i)
    {
        if (quads[i].minX <= x && x < quads[i].maxX
            && quads[i].minY <= y && y < quads[i].maxY)
        {
            return &quads[i];
        }
    }
    return nullptr;
}

std::vector<std::uint16_t> WaterQuad::idsAt(std::int16_t x, std::int16_t y)
{
    std::vector<std::uint16_t> res;
    WaterQuad* quads = getArray();
    if (!quads) return res;
    const std::uint16_t count = getCount();
    for (std::uint16_t i = 0; i < count; ++i)
    {
        if (quads[i].minX <= x && x < quads[i].maxX
            && quads[i].minY <= y && y < quads[i].maxY)
        {
            res.emplace_back(i);
        }
    }
    return res;
}

WaterQuad* WaterQuad::get(std::uint16_t id)
{
    WaterQuad* quads = getArray();
    if (!quads) return nullptr;
    return &quads[id];
}

std::uint16_t WaterQuad::size()
{
    return getCount();
}
