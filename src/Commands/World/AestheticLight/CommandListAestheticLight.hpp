#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandColourRGB.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/World/AestheticLight/CommandAestheticLightPlacement.hpp"
#include "Commands/World/AestheticLight/CommandAestheticLight.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListAestheticLight : public CommandList
    {
    public:
        explicit CommandListAestheticLight(CommandList* parent)
            : CommandList(parent, LIT("Aesthetic Light"))
        {
            auto colour    = makeChild<CommandColourRGB>(LIT("Colour"), CMDNAMES("aestheticcolour"), NOLABEL, 255, 0, 255);
            auto placement = makeChild<CommandAestheticLightPlacement>();
            auto range     = makeChild<CommandSliderFloat>(LIT("Range"),       CMDNAMES("aestheticrange"),       NOLABEL,        0, 1000000, 900000, 100);
            auto intensity = makeChild<CommandSliderFloat>(LIT("Intensity"),   CMDNAMES("aestheticintensity"),   NOLABEL,        0, 1000000,   1000, 100);
            auto shadow    = makeChild<CommandSliderFloat>(LIT("Diffraction"), CMDNAMES("aestheticdiffraction"), NOLABEL, -1000000, 1000000,      0, 100);

            auto rainbow = makeChild<CommandVehRainbow>(
                CMDNAMES("aestheticrainbow"),
                [col = colour.get()](RGB rgb) { col->m_r = rgb.r; col->m_g = rgb.g; col->m_b = rgb.b; }
            );

            createChild<CommandAestheticLight>(colour.get(), placement.get(), range.get(), intensity.get(), shadow.get());

            children.emplace_back(std::move(colour));
            children.emplace_back(std::move(rainbow));
            children.emplace_back(std::move(placement));
            children.emplace_back(std::move(range));
            children.emplace_back(std::move(intensity));
            children.emplace_back(std::move(shadow));
        }
    };
}
