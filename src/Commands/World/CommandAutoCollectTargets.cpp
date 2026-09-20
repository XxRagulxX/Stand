#include "Commands/LoopedCommand.hpp"
#include "Scripting/ScriptLocal.hpp"


namespace Stand::Features
{
    class AutoCollectTargets : public LoopedCommand
    {
        using LoopedCommand::LoopedCommand;
        
        virtual void OnTick() override
        {
            if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
            {
                auto local = ScriptLocal(thread, 10713);
                auto lootState = local.as<int*>();
                if (lootState && *lootState == 3)
                {
                    *lootState = 4;
                    *local.at(14).as<float*>() = 2.0f;
                }
            }
        }
    };

    static AutoCollectTargets _AutoCollectTargets{"autocollecttargets", "Auto Collect Targets", "Automatically Collects Loots"};
}