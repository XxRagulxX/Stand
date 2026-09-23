#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    class CommandListRocketBoost : public CommandList
    {
    public:
        explicit CommandListRocketBoost(CommandList* parent);
    };
}
