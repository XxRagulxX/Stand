#pragma once
#include "Commands/Widgets/CommandSliderProximity.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
    class CommandPunishableProximity : public CommandSliderProximity
    {
    public:
        explicit CommandPunishableProximity(CommandList* parent)
            : CommandSliderProximity(parent, CMDNAMES("punishableproximity"))
        {}

        void onChange(Click& click, int prev_value) final
        {
            AllEntitiesEveryTick::npc_punishable_proximity = getFloatValue();
        }
    };
}
