#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/CommandTabVehicle.hpp"

namespace Stand::Features
{
    Stand::CommandTabSpawnOnFoot& GetCommandTabSpawnOnFoot()
    {
        return *GetCommandTabVehicle().spawnOnFoot;
    }
}
