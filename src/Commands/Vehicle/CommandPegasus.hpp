#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandPegasus : public CommandPhysical
    {
    public:
        explicit CommandPegasus(CommandList* parent);
        void onClick(Click& click) override;
    };
}
