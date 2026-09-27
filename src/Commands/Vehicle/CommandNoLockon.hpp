#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandNoLockon : public CommandToggle
    {
    public:
        explicit CommandNoLockon(CommandList* parent);
        void onChange(Click& click) override;
    };
}
