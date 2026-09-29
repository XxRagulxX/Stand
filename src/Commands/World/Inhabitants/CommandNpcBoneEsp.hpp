#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandNpcBoneEsp : public CommandToggle
    {
    public:
        explicit CommandNpcBoneEsp(CommandList* parent)
            : CommandToggle(parent, LIT("Bone ESP"), CMDNAMES("npcboneesp", "npcbonesp"))
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::npc_bone_esp = m_on;
        }
    };
}
