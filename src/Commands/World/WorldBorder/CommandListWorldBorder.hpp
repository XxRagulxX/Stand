#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandColourRGB.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/World/WorldBorder/CommandWorldBorder.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListWorldBorder : public CommandList
    {
    public:
        explicit CommandListWorldBorder(CommandList* parent)
            : CommandList(parent, LIT("World Border"))
        {
            auto colour = makeChild<CommandColourRGB>(LIT("Colour"), CMDNAMES("worldborderhex"), NOLABEL, 255, 0, 255);

            auto rainbow = makeChild<CommandVehRainbow>(
                CMDNAMES("worldborderrainbow"),
                [col = colour.get()](RGB rgb) { col->m_r = rgb.r; col->m_g = rgb.g; col->m_b = rgb.b; }
            );

            createChild<CommandWorldBorder>(colour.get());

            children.emplace_back(std::move(colour));
            children.emplace_back(std::move(rainbow));
        }
    };
}
