#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandTpWp : public CommandPhysical
    {
    public:
        explicit CommandTpWp(CommandList* parent);
        void onClick(Click& click) override;
    };
}
