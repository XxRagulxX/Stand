#include "Game/WaterQuad.hpp"
#include "Core/Pointers.hpp"

WaterQuad* WaterQuad::at(std::int16_t x, std::int16_t y)
{
    if (!Stand::Pointers.water_quads || !Stand::Pointers.water_quads_size)
        return nullptr;
    WaterQuad* quads = *Stand::Pointers.water_quads;
    const std::uint16_t count = *Stand::Pointers.water_quads_size;
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
    if (!Stand::Pointers.water_quads || !Stand::Pointers.water_quads_size)
        return res;
    WaterQuad* quads = *Stand::Pointers.water_quads;
    const std::uint16_t count = *Stand::Pointers.water_quads_size;
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
    if (!Stand::Pointers.water_quads)
        return nullptr;
    return &(*Stand::Pointers.water_quads)[id];
}

std::uint16_t WaterQuad::size()
{
    if (!Stand::Pointers.water_quads_size)
        return 0;
    return *Stand::Pointers.water_quads_size;
}
