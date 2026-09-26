#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListCountermeasures : public CommandList
    {
    public:
        explicit CommandListCountermeasures(CommandList* parent);
    };
}
