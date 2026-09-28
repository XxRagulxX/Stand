#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandWpObjective : public CommandPhysical
    {
    public:
        explicit CommandWpObjective(CommandList* parent);
        void onClick(Click& click) override;
    };
}
