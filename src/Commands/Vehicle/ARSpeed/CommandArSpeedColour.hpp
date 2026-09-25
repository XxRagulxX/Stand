#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListArSpeedColour : public CommandList
    {
    public:
        int m_r = 255;
        int m_g = 0;
        int m_b = 255;
        int m_a = 255;

        explicit CommandListArSpeedColour(CommandList* parent);
    };
}
