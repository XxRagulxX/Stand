#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListAtmosphere : public CommandList
    {
    public:
        explicit CommandListAtmosphere(CommandList* parent);
    };
}
