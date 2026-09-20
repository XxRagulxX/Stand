#include "Commands/Vehicle/PersonalVehicles/CommandPersonalVehicles.hpp"

#include "Commands/CommandInput.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Vehicle/GarageVehicleMgr.hpp"
#include "Vehicle/PersonalVehicles.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
    std::size_t g_PVVersion = 0;

    namespace
    {
        PVSort s_Sort = PVSort::Name;

        class CommandFindPV : public CommandInput
        {
        public:
            CommandFindPV()
                : CommandInput("findpv", "Search", "Filter personal vehicles by name.", "") {}

            void OnChange() override { ++g_PVVersion; }

            void OnCall() override
            {
                Rendering::MenuCommandBox::Open(
                    "findpv",
                    "Search Personal Vehicles",
                    "Filter by name.",
                    m_StringValue,
                    [this](const std::string& text) -> bool {
                        SetStringValue(text);
                        return true;
                    }
                );
            }
        };
        static CommandFindPV s_FindPV;

        static StandWidgets::CommandLambdaAction s_RefreshPVs{
            "pvs",
            "Personal Vehicles",
            "Refreshes the personal vehicle list.",
            [] { PersonalVehicles::Update(); ++g_PVVersion; }
        };

        static StandWidgets::CommandLambdaAction s_SaveAllPVs{
            "saveallpvs",
            "Save All To Garage",
            "Saves the data of all your personal vehicles to Stand's garage.",
            [] { FiberPool::queueJob([] { SaveAllPVsToGarage(); }); }
        };
    }

    std::string GetPVFilter()
    {
        return s_FindPV.GetString();
    }

    PVSort GetPVSort()
    {
        return s_Sort;
    }

    void SetPVSort(PVSort sort)
    {
        s_Sort = sort;
        ++g_PVVersion;
    }

    void OpenPVSearch()
    {
        Rendering::MenuCommandBox::Open(
            "findpv",
            "Search Personal Vehicles",
            "Filter by name.",
            s_FindPV.GetString(),
            [](const std::string& text) -> bool {
                s_FindPV.SetStringValue(text);
                return true;
            }
        );
    }

    void SaveAllPVsToGarage()
    {
        auto& pvMap = PersonalVehicles::GetPersonalVehicles();
        auto pedPos = Self::GetPed().GetPosition();
        const rage::fvector3 coords{ pedPos.x + 10.0f, pedPos.y, pedPos.z };
        const float heading = Self::GetPed().GetHeading();

        for (auto& [rawName, pv] : pvMap)
        {
            auto veh = pv->Clone(coords, heading);
            if (!veh.IsValid())
                continue;

            const std::string saveName = rawName.substr(0, rawName.find("##"));
            GarageVehicleMgr::Save(saveName, veh.GetHandle());

            int h = veh.GetHandle();
            ENTITY::DELETE_ENTITY(&h);

            Script::current()->yield(100);
        }
    }
}
