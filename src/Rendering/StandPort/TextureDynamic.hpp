#pragma once
#include <cstdint>
#include <functional>

namespace Stand
{
    struct TextureDynamic
    {
        uint32_t width = 0;
        uint32_t height = 0;

        [[nodiscard]] bool hasUnderlyingTexture() const noexcept
        {
            return width > 0 && height > 0;
        }

        void ensureHasUnderlyingTextureOfSize(uint32_t w, uint32_t h) noexcept
        {
            width = w;
            height = h;
        }

        void drawOnto(const std::function<void()>& draw_func, bool = false) const
        {
            draw_func();
        }

        void reset() noexcept
        {
            width = height = 0;
        }
    };
}
