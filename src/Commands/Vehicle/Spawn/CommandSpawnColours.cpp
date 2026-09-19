#include "Commands/Vehicle/Spawn/CommandSpawnColours.hpp"

#include "Commands/CommandColourCustom.hpp"

namespace Stand::Features
{
    namespace
    {
        static Stand::CommandColourCustom s_SpawnPrimary{
            "spawnprimarycolour",
            "Primary Colour",
            "The primary colour applied to vehicles spawned via Stand when \"Colour Spawned Vehicles\" is enabled.",
            ImVec4(1.f, 1.f, 1.f, 1.f)
        };

        static Stand::CommandColourCustom s_SpawnSecondary{
            "spawnsecondarycolour",
            "Secondary Colour",
            "The secondary colour applied to vehicles spawned via Stand when \"Colour Spawned Vehicles\" is enabled.",
            ImVec4(1.f, 1.f, 1.f, 1.f)
        };
    }

    ImVec4 GetSpawnPrimaryColour()
    {
        return s_SpawnPrimary.GetState();
    }

    ImVec4 GetSpawnSecondaryColour()
    {
        return s_SpawnSecondary.GetState();
    }
}
