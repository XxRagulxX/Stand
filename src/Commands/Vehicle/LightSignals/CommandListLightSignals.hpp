#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    class CommandListLightSignals : public CommandList
    {
    public:
        explicit CommandListLightSignals(CommandList* parent);
    };
}
