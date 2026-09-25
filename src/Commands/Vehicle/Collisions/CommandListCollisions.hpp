#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListCollisions : public CommandList
    {
    public:
        explicit CommandListCollisions(CommandList* parent);
    };
}
