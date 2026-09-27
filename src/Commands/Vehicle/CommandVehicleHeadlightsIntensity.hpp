#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"

namespace Stand
{
    class CommandVehicleHeadlightsIntensity : public CommandSliderFloat
    {
        bool m_ticking = false;
    public:
        explicit CommandVehicleHeadlightsIntensity(CommandList* parent);
        void onChange(Click& click, int prev_value) override;
        void onTick() override;
        ~CommandVehicleHeadlightsIntensity() override;
    };
}
