#pragma once

#include "Rendering/StandPort/Position2d.hpp"

namespace Stand
{
    struct StandMenuGrid
    {
        Position2d origin{ 1323, 560 };
        int16_t spacer_size = 3;

        void update() {}
    };

    inline StandMenuGrid g_menu_grid{};
}
