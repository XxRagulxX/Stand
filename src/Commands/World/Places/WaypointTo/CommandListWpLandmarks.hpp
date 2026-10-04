#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/WaypointTo/CommandWpToCoord.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListWpLandmarks : public CommandList
    {
    public:
        explicit CommandListWpLandmarks(CommandList* parent)
            : CommandList(parent, LIT("Landmarks"))
        {
            createChild<CommandWpToCoord>(LIT("LSIA"),           -1087.743400f, -3015.614000f);
            createChild<CommandWpToCoord>(LIT("Maze Bank"),         -75.218800f,  -818.582000f);
            createChild<CommandWpToCoord>(LIT("Fort Zancudo"),    -2285.929400f,  3124.115000f);
            createChild<CommandWpToCoord>(LIT("Mount Chiliad"),     501.770320f,  5595.622000f);
            createChild<CommandWpToCoord>(LIT("Cayo Perico"),      4906.255400f, -4912.764600f);
            createChild<CommandWpToCoord>(LIT("Diamond Casino"),    922.816223f,    47.206078f);
        }
    };
}
