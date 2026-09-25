#pragma once
#include "Commands/CommandInput.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Vehicle/GarageVehicleMgr.hpp"

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

            void OnCall() override
            {
                Rendering::MenuCommandBox::Open(
                    "findsaved",
                    "Search Garage",
                    "Filter saved vehicles by name.",
                    m_StringValue,
                    [this](const std::string& text) -> bool {
                        SetStringValue(text);
                        return true;
                    }
                );
            }
        };
        static CommandFindSaved s_FindSaved;
    }
}
