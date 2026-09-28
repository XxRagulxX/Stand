#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/TeleportTo/CommandTpToCoord.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListLandmarks : public CommandList
    {
    public:
        explicit CommandListLandmarks(CommandList* parent)
            : CommandList(parent, LIT("Landmarks"))
        {
            createChild<CommandTpToCoord>(LIT("LSIA"),           -1037.7470f,  -2738.5270f,   20.1690f);
            createChild<CommandTpToCoord>(LIT("Maze Bank"),          -75.0150f,   -818.2120f,  326.2055f);
            createChild<CommandTpToCoord>(LIT("Fort Zancudo"),     -2047.8590f,   3132.5060f,   32.8136f);
            createChild<CommandTpToCoord>(LIT("Mount Chiliad"),      501.5510f,   5604.7200f,  797.8756f);
            createChild<CommandTpToCoord>(LIT("Cayo Perico"),       4701.1890f,  -5143.0890f,    2.0000f);
            createChild<CommandTpToCoord>(LIT("Diamond Casino"),    924.5790f,     47.8040f,   80.9079f);
        }
    };
}
