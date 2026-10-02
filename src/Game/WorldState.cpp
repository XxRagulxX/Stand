#include "Game/WorldState.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
    BOOL WorldState::getOnline()
    {
        return *ScriptGlobal(GLOBAL_MP_WORLD_STATE).as<BOOL*>();
    }

    void WorldState::setOnline(BOOL toggle)
    {
        if (getOnline() != toggle)
        {
            *ScriptGlobal(GLOBAL_MP_WORLD_STATE).as<BOOL*>() = toggle;
            if (toggle)
                DLC::ON_ENTER_MP();
            else
                DLC::ON_ENTER_SP();
        }
    }
}
