#pragma once
#include "Game/typedecl.hpp"

namespace Stand
{
    struct tbFreecam
    {
        v3 pos;
        [[nodiscard]] bool isEnabled() const noexcept { return false; }
        [[nodiscard]] bool canMovementCommandPerformMovement() const noexcept { return true; }
    };

    inline tbFreecam g_tb_freecam{};
}
