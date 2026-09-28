#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandAutoTpWp : public CommandToggle
    {
    public:
        explicit CommandAutoTpWp(CommandList* parent);
        void onChange(Click& click) override;
    };
}
