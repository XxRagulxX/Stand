#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandToggleEngine : public CommandPhysical
    {
    public:
        explicit CommandToggleEngine(CommandList* parent);
        void onClick(Click& click) override;
    };
}
