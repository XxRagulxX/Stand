#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandExitStop : public CommandToggle
    {
    public:
        explicit CommandExitStop(CommandList* parent);
        void onChange(Click& click) override;
    };
}
