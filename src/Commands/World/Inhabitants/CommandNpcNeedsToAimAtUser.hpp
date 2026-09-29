#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
    class CommandNpcNeedsToAimAtUser : public CommandToggle
    {
    public:
        explicit CommandNpcNeedsToAimAtUser(CommandList* parent)
            : CommandToggle(parent, LIT("Punish Only If Aiming At Me"), {}, LIT("If disabled, NPCs will be punished for aiming in your area, regardless of who they're aiming at."), true)
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::npc_needs_to_aim_at_user = m_on;
        }
    };
}
