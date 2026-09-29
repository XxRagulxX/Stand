#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTrafficColour : public CommandColourList
    {
    public:
        int m_r = 255;
        int m_g = 0;
        int m_b = 255;

        explicit CommandTrafficColour(CommandList* parent);
    };
}
