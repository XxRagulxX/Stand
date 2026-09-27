#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandFlip : public CommandPhysical
    {
    public:
        explicit CommandFlip(CommandList* parent);
        void onClick(Click& click) override;
    };
}
