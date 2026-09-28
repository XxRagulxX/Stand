#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListIPLs : public CommandList
    {
    public:
        explicit CommandListIPLs(CommandList* parent);
    };
}
