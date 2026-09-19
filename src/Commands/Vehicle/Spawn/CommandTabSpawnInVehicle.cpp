#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"
#include "Commands/Vehicle/CommandTabVehicle.hpp"

namespace Stand::Features
{
    Stand::CommandTabSpawnInVehicle& GetCommandTabSpawnInVehicle()
    {
        return *GetCommandTabVehicle().spawnInVehicle;
    }
}
