#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandLeaveEngineRunning : public CommandToggle
    {
    public:
        explicit CommandLeaveEngineRunning(CommandList* parent);
        void onChange(Click& click) override;
    };
}
