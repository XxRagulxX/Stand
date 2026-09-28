#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandTpClipboard : public CommandPhysical
    {
    public:
        explicit CommandTpClipboard(CommandList* parent);
        void onClick(Click& click) override;
    };
}
