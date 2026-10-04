#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListWater : public CommandList
    {
    public:
        explicit CommandListWater(CommandList* parent);
    };
}
