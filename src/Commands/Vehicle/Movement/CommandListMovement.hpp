#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    class CommandListMovement : public CommandList
    {
    public:
        explicit CommandListMovement(CommandList* parent);
    };
}
