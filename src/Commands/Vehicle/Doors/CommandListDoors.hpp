#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListDoors : public CommandList
    {
    public:
        explicit CommandListDoors(CommandList* parent);
    };
}
