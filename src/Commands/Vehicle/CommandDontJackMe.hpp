#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandDontJackMe : public CommandToggle
    {
    public:
        explicit CommandDontJackMe(CommandList* parent);
        void onChange(Click& click) override;
    };
}
