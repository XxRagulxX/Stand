#pragma once
#include "Commands/CommandInput.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Vehicle/GarageVehicleMgr.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
    namespace
    {
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

            void OnCall() override
            {
                Rendering::MenuCommandBox::Open(
                    "savevehicle",
                    "Save Vehicle",
                    "Enter a name for your current vehicle.",
                    "",
                    [this](const std::string& text) -> bool {
                        if (text.empty())
                            return false;
                        SetStringValue(text);
                        return true;
                    }
                );
            }
        };
        static CommandSaveVehicle s_SaveVehicle;
    }
}
