#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListInhabitants : public CommandList
    {
    public:
        explicit CommandListInhabitants(CommandList* parent);
    };
}
