#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

#include <vector>

namespace Stand
{
    class CommandEspColourPicker : public CommandList
    {
    public:
        CommandEspColourPicker(CommandList* parent, Label name, std::vector<CommandName> cmdnames,
                               int* r, int* g, int* b);
    };
}
