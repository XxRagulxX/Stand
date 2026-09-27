#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

#include <string>

namespace Stand
{
    class CommandVehicleSeat : public CommandSlider
    {
    public:
        explicit CommandVehicleSeat(CommandList* parent);
        std::string getValueText() const override;
        void onChange(Click& click, int prev_value) override;
    };
}
