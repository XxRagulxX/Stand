#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
    class CommandPlayerNeedsToAimAtUser : public CommandToggle
    {
    public:
        explicit CommandPlayerNeedsToAimAtUser(CommandList* parent)
            : CommandToggle(parent, LIT("Punish Only If Aiming At Me"), {}, LIT("If disabled, players will be punished for aiming in your area, regardless of who they're aiming at."), true)
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::player_needs_to_aim_at_user = m_on;
        }
    };
}
