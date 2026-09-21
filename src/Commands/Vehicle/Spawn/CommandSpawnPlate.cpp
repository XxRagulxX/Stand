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
        const std::string current = s_SpawnPlate.GetString();
        Rendering::MenuCommandBox::Open(
            "spawnplate",
            "Spawned Vehicles License Plate",
            std::to_string(current.size()) + "/8",
            current,
            [](const std::string& text) -> bool {
                if (text.length() > 8)
                    return false;
                s_SpawnPlate.SetStringValue(text);
                return true;
            },
            // onType: live character counter
            [](const std::string& text) -> std::string {
                return std::to_string(text.size()) + "/8";
            },
            // onTypeLabel: live plate preview
            [](const std::string& text) -> std::string {
                return "Spawned Vehicle License Plate: " + (text.empty() ? std::string("(random)") : text);
            }
        );
    }
}
