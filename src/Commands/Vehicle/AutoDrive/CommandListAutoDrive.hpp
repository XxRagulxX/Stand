#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    class CommandListAutoDrive : public CommandList
    {
    public:
        explicit CommandListAutoDrive(CommandList* parent);
    };
}
