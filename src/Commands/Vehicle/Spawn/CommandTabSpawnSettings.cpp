#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/CommandTabVehicle.hpp"

namespace Stand::Features
{
    Stand::CommandTabSpawnSettings& GetCommandTabSpawnSettings()
    {
        return *GetCommandTabVehicle().spawnSettings;
    }
}
