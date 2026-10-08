#pragma once

#include "Rendering/StandPort/GridItemText.hpp"
#include "Rendering/StandPort/TextSettings.hpp"

#include <string>

namespace Stand::Rendering
{
    class GridItemPrimaryText : public GridItemText
    {
    protected:
        std::wstring text;

    public:
        explicit GridItemPrimaryText(int16_t width, int16_t height, std::wstring text);

        void draw() override;

    protected:
        void draw(const TextSettings& settings);
    };
}
