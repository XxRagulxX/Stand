#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTrains : public CommandToggle
    {
    public:
        explicit CommandTrains(CommandList* parent);

        void onEnable(Click& click) final;
        void onDisable(Click& click) final;
    };
}
