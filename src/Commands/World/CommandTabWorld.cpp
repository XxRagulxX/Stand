#include "Commands/World/CommandTabWorld.hpp"

namespace Stand::Features
{
    Stand::CommandTabWorld& GetCommandTabWorld()
    {
        static Stand::CommandTabWorld instance{};
        return instance;
    }
}
