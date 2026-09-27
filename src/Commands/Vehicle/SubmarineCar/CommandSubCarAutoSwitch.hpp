#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandSubCarAutoSwitch : public CommandToggle
    {
    public:
        CommandToggle* config_noAnim = nullptr;

        explicit CommandSubCarAutoSwitch(CommandList* parent);
        void onChange(Click& click) override;
    };
}
