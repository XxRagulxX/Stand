#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandWpClipboard : public CommandPhysical
    {
    public:
        explicit CommandWpClipboard(CommandList* parent);
        void onClick(Click& click) override;
    };
}
