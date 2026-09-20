#include "Commands/Vehicle/Garage/CommandGarage.hpp"

#include "Commands/CommandInput.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Core/FileMgr.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Vehicle/GarageVehicleMgr.hpp"
#include "World/Self.hpp"

#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand::Features
{
    namespace
    {
        class CommandFindSaved : public CommandInput
        {
        public:
            CommandFindSaved()
                : CommandInput("findsaved", "Search", "Filter your saved vehicles by name.", "") {}

            void OnChange() override
            {
                ++GarageVehicleMgr::s_Version;
            }
        };
        static CommandFindSaved s_FindSaved;

        class CommandSaveVehicle : public CommandInput
        {
        public:
            CommandSaveVehicle()
                : CommandInput("savevehicle", "Save Vehicle",
                    "Save your current vehicle to the garage with the given name.", "") {}

            void OnChange() override
            {
                if (m_StringValue.empty())
                    return;
                const auto name = m_StringValue;
                FiberPool::queueJob([name] {
                    const int pedH = Self::GetPed().GetHandle();
                    if (!PED::IS_PED_IN_ANY_VEHICLE(pedH, false))
                    {
                        Notifications::Show("Garage", "You are not in a vehicle.", NotificationType::Warning);
                        return;
                    }
                    const int vehH = PED::GET_VEHICLE_PED_IS_IN(pedH, false);
                    GarageVehicleMgr::Save(name, vehH);
                    Notifications::Show("Garage", "Vehicle saved: " + name);
                });
            }
        };
        static CommandSaveVehicle s_SaveVehicle;

        static StandWidgets::CommandLambdaAction s_OpenGarageFolder{
            "opengaragefolder",
            "Open Folder",
            "Opens the folder containing your saved garage vehicles.",
            [] {
                auto path = FileMgr::GetProjectFolder("./Vehicles").Path();
                std::filesystem::create_directories(path);
                ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            }
        };
    }

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
