#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandUndoTp : public CommandPhysical
    {
    public:
        explicit CommandUndoTp(CommandList* parent);
        void onClick(Click& click) override;
    };
}
