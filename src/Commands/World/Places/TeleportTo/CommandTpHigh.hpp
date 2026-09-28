#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandTpHigh : public CommandPhysical
    {
    public:
        explicit CommandTpHigh(CommandList* parent);
        void onClick(Click& click) override;
    };
}
