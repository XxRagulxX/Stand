#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandInstaSeat : public CommandToggle
    {
        bool m_gaveExitTask = false;
    public:
        explicit CommandInstaSeat(CommandList* parent);
        void onChange(Click& click) override;
    };
}
