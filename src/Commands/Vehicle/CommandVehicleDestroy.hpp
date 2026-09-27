#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandVehicleDestroy : public CommandPhysical
    {
    public:
        explicit CommandVehicleDestroy(CommandList* parent);
        void onClick(Click& click) override;
    };
}
