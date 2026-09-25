#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListArSpeed : public CommandList
    {
    public:
        bool m_use_dx = false;

        explicit CommandListArSpeed(CommandList* parent);
    };
}
