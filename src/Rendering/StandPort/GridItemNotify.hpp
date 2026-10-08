#pragma once

#include "Rendering/StandPort/GridItemText.hpp"

#include <ctime>
#include <string>

namespace Stand::Rendering
{
    class GridItemNotify : public GridItemText
    {
    public:
        const time_t show_until;
        time_t flash_until;
        bool frozen;
        bool flashing;

        explicit GridItemNotify(const std::wstring& text, time_t show_until, time_t flash_until);

        void draw() override;
    };
}
