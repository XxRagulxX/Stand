#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandMint : public CommandToggle
    {
    public:
        explicit CommandMint(CommandList* parent);
        void onChange(Click& click) override;
    };
}
