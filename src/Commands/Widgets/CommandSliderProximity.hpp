#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"

namespace Stand
{
    class CommandSliderProximity : public CommandSliderFloat
    {
    public:
        explicit CommandSliderProximity(CommandList* parent, std::vector<CommandName>&& command_names, int default_value = 1000)
            : CommandSliderFloat(parent, LIT("Proximity"), std::move(command_names), NOLABEL, 0, 100000, default_value, 100)
        {}
    };
}
