#include "Commands/World/CommandTrains.hpp"

#include "Scripting/Natives.hpp"

namespace Stand
{
    CommandTrains::CommandTrains(CommandList* parent)
        : CommandToggle(parent, LIT("I Like Trains"), CMDNAMES("trains", "iliketrains"),
            LIT("Makes trains much more common in the world."))
    {
    }

    void CommandTrains::onEnable(Click& click)
    {
        ensureScriptThread(click, []
        {
            for (int i = 0; i <= 26; i++)
                VEHICLE::SET_TRAIN_TRACK_SPAWN_FREQUENCY(i, 0);
        });
    }

    void CommandTrains::onDisable(Click& click)
    {
        ensureScriptThread(click, []
        {
            for (int i = 0; i <= 26; i++)
                VEHICLE::SET_TRAIN_TRACK_SPAWN_FREQUENCY(i, 120000);
        });
    }
}
