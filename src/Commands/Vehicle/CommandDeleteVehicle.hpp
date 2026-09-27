#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandDeleteVehicle : public CommandPhysical
    {
    public:
        explicit CommandDeleteVehicle(CommandList* parent);
        void onClick(Click& click) override;
    };
}
