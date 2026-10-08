#pragma once

#include <cstdint>

namespace Stand
{
    struct NativeCallContext
    {
        [[nodiscard]] static bool canInvoke(uint64_t) noexcept { return false; }
    };
}
