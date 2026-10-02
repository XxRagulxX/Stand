#pragma once
#include <cstdint>
#include <vector>

struct WaterOpacityData
{
    std::uint8_t a1;
    std::uint8_t a2;
    std::uint8_t a3;
    std::uint8_t a4;
};
static_assert(sizeof(WaterOpacityData) == 4);

struct WaterQuad
{
    std::int16_t minX;
    std::int16_t minY;
    std::int16_t maxX;
    std::int16_t maxY;
    WaterOpacityData opacity;
    float unk;
    std::uint8_t pad_0010[4];
    float z;
    std::uint8_t unk_0018;
    std::uint8_t Type;
    std::uint8_t pad_001A[2];

    [[nodiscard]] static WaterQuad* at(std::int16_t x, std::int16_t y);
    [[nodiscard]] static WaterQuad* at(float x, float y) { return at(static_cast<std::int16_t>(x), static_cast<std::int16_t>(y)); }

    [[nodiscard]] static std::vector<std::uint16_t> idsAt(std::int16_t x, std::int16_t y);
    [[nodiscard]] static std::vector<std::uint16_t> idsAt(float x, float y) { return idsAt(static_cast<std::int16_t>(x), static_cast<std::int16_t>(y)); }

    [[nodiscard]] static WaterQuad* get(std::uint16_t id);
    [[nodiscard]] static std::uint16_t size();
};
static_assert(sizeof(WaterQuad) == 0x1C);
