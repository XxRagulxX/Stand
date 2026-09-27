#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehGod : public CommandToggle
    {
        int m_lastVeh = 0;
    public:
        explicit CommandVehGod(CommandList* parent);
        void onChange(Click& click) override;
    };
}
