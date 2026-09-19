#include "Commands/Vehicle/Spawn/CommandSpawnPlate.hpp"

#include "Commands/CommandInput.hpp"
#include "Rendering/MenuCommandBox.hpp"

namespace Stand::Features
{
    namespace
    {
        static Stand::CommandInput s_SpawnPlate{
            "spawnplate",
            "Spawned Vehicles License Plate",
            "The license plate text applied to vehicles spawned via Stand. Leave empty for a random plate. Max 8 characters.",
            ""
        };
    }

    std::string GetSpawnPlateText()
    {
        return s_SpawnPlate.GetString();
    }

    void OpenSpawnPlate()
    {
        Rendering::MenuCommandBox::Open(
            "spawnplate",
            "Spawned Vehicles License Plate",
            "Max 8 characters. Leave empty for a random plate.",
            s_SpawnPlate.GetString(),
            [](const std::string& text) -> bool {
                if (text.length() > 8)
                    return false;
                s_SpawnPlate.SetStringValue(text);
                return true;
            }
        );
    }
}
