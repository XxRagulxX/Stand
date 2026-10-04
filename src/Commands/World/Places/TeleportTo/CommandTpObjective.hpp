#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandTpObjective : public CommandPhysical
    {
    public:
        explicit CommandTpObjective(CommandList* parent);
        void onClick(Click& click) override;
    };
}
