#include "Commands/Vehicle/Garage/CommandGarage.hpp"

#include "Commands/Vehicle/Garage/CommandFindSaved.hpp"
#include "Commands/Vehicle/Garage/CommandSaveVehicle.hpp"
#include "Commands/Vehicle/Garage/CommandOpenGarageFolder.hpp"

#include "Rendering/MenuCommandBox.hpp"
#include "Core/FileMgr.hpp"

#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand::Features
{
    std::string GetGarageFilter()
    {
        return s_FindSaved.GetString();
    }

    void OpenGarageSearch()
    {
        Rendering::MenuCommandBox::Open(
            "findsaved",
            "Search Garage",
            "Filter saved vehicles by name.",
            s_FindSaved.GetString(),
            [](const std::string& text) -> bool {
                s_FindSaved.SetStringValue(text);
                return true;
            }
        );
    }

    void OpenSaveVehicle()
    {
        Rendering::MenuCommandBox::Open(
            "savevehicle",
            "Save Vehicle",
            "Enter a name for your current vehicle.",
            "",
            [](const std::string& text) -> bool {
                if (text.empty())
                    return false;
                s_SaveVehicle.SetStringValue(text);
                return true;
            }
        );
    }

    void OpenGarageFolder()
    {
        auto path = FileMgr::GetProjectFolder("./Vehicles").Path();
        std::filesystem::create_directories(path);
        ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    }
}
