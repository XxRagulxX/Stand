#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandFixVehicle : public CommandPhysical
    {
    public:
        explicit CommandFixVehicle(CommandList* parent);
        void onClick(Click& click) override;
    };
}
