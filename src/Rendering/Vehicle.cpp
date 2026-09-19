#include "Rendering/Vehicle.hpp"

#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Vehicle/VehicleEntityPreview.hpp"
#include "Vehicle/VehicleCommandBox.hpp"
#include "Vehicle/SpawnedVehicleListGrid.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Vehicle/VehicleData.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Stand::Rendering
{
    namespace
    {
        // ── DLC Grid ─────────────────────────────────────────────────────────

        std::vector<std::string> g_DlcOrder;

        void BuildDlcOrder()
        {
            static bool built = [] {
                for (size_t i = 0; i < g_VehicleCount; ++i)
                {
                    std::string dlc(g_VehicleData[i].dlc_name);
                    bool found = false;
                    for (const auto& d : g_DlcOrder)
                        if (d == dlc) { found = true; break; }
                    if (!found)
                        g_DlcOrder.push_back(dlc);
                }
                return true;
            }();
        }

        class DlcCategoryGrid : public Grid
        {
            std::string m_Dlc;
        public:
            explicit DlcCategoryGrid(std::string dlc)
                : Grid(Theme::GetContentOrigin(), 0), m_Dlc(std::move(dlc))
            {
            }
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                for (size_t i = 0; i < g_VehicleCount; ++i)
                {
                    if (m_Dlc != g_VehicleData[i].dlc_name)
                        continue;
                    items_draft.push_back(std::make_unique<VehicleEntityPreview>(
                        Theme::kContentWidth, h, Stand::Joaat(g_VehicleData[i].hash_name)));
                }
            }
        };

        std::vector<std::unique_ptr<DlcCategoryGrid>> g_DlcCategoryGrids;

        class VehicleSpawnDlcGrid : public Grid
        {
        public:
            VehicleSpawnDlcGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                BuildDlcOrder();

                if (g_DlcCategoryGrids.empty())
                {
                    g_DlcCategoryGrids.reserve(g_DlcOrder.size());
                    for (const auto& dlc : g_DlcOrder)
                        g_DlcCategoryGrids.push_back(std::make_unique<DlcCategoryGrid>(dlc));
                }

                for (size_t i = 0; i < g_DlcOrder.size(); ++i)
                {
                    items_draft.push_back(std::make_unique<GridItemFolder>(
                        Theme::kContentWidth,
                        static_cast<int16_t>(Theme::kContentItemHeight),
                        g_DlcOrder[i],
                        g_DlcCategoryGrids[i].get()
                    ));
                }
            }
        };

        // ── Class Grid ────────────────────────────────────────────────────────

        static constexpr const char* kClassDisplayNames[23] = {
            "Compacts", "Sedans", "SUVs", "Coupes", "Muscle",
            "Sports Classic", "Sports", "Super", "Motorcycles", "Off-Road",
            "Industrial", "Utility", "Vans", "Cycles", "Boats",
            "Helicopters", "Planes", "Service", "Emergency", "Military",
            "Commercial", "Trains", "Open Wheel",
        };

        std::array<std::vector<joaat_t>, 23> g_ClassBuckets;

        void BuildClassBuckets()
        {
            static bool built = [] {
                for (size_t i = 0; i < g_VehicleCount; ++i)
                {
                    const joaat_t hash = Stand::Joaat(g_VehicleData[i].hash_name);
                    const int cls = VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(hash);
                    if (cls >= 0 && cls < 23)
                        g_ClassBuckets[cls].push_back(hash);
                }
                return true;
            }();
        }

        class ClassVehicle : public Grid
        {
            int m_ClassIdx;
        public:
            explicit ClassVehicle(int classIdx)
                : Grid(Theme::GetContentOrigin(), 0), m_ClassIdx(classIdx)
            {
            }
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                for (auto hash : g_ClassBuckets[m_ClassIdx])
                    items_draft.push_back(std::make_unique<VehicleEntityPreview>(Theme::kContentWidth, h, hash));
            }
        };

        std::vector<std::unique_ptr<ClassVehicle>> g_ClassGrids;

        class VehicleSpawnClassGrid : public Grid
        {
        public:
            VehicleSpawnClassGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                BuildClassBuckets();

                if (g_ClassGrids.empty())
                {
                    g_ClassGrids.reserve(23);
                    for (int i = 0; i < 23; ++i)
                        g_ClassGrids.push_back(std::make_unique<ClassVehicle>(i));
                }

                for (int i = 0; i < 23; ++i)
                {
                    if (g_ClassBuckets[i].empty())
                        continue;
                    items_draft.push_back(std::make_unique<GridItemFolder>(
                        Theme::kContentWidth,
                        static_cast<int16_t>(Theme::kContentItemHeight),
                        kClassDisplayNames[i],
                        g_ClassGrids[i].get()
                    ));
                }
            }
        };

        // ── On Foot Behaviour Grid ────────────────────────────────────────────

        class VehicleSpawnOnFootGrid : public Grid
        {
        public:
            VehicleSpawnOnFootGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabSpawnOnFoot();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.drivespawned));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnair));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnfront));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.likepv));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.deleteprevious));
            }
        };

        // ── In Vehicle Behaviour Grid ─────────────────────────────────────────

        class VehicleSpawnInVehicle : public Grid
        {
        public:
            VehicleSpawnInVehicle() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabSpawnInVehicle();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.drivespawned));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnair));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnfront));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.likepv));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.deleteprevious));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.keepvelocity));
            }
        };

        // ── Spawn Grid ────────────────────────────────────────────────────────

        VehicleSpawnDlcGrid       g_DlcContent{};
        VehicleSpawnClassGrid     g_ClassContent{};
        SpawnedVehicleListGrid    g_SpawnList{};
        VehicleSpawnOnFootGrid    g_OnFootContent{};
        VehicleSpawnInVehicle g_InVehicleContent{};

        class VehicleSpawnGrid : public Grid
        {
        public:
            VehicleSpawnGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t kItemH = static_cast<int16_t>(Theme::kContentItemHeight);

                auto& settings = Features::GetCommandTabSpawnSettings();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, settings.manufacturerNames));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, settings.spawngod));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, settings.spawntune));
                items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Search", Stand::OpenVehicleSearch));
                items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Input Model Name", Stand::OpenInputModelName));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Classes",                  &g_ClassContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "DLCs",                     &g_DlcContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "On Foot Behaviour",        &g_OnFootContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "In Vehicle Behaviour",     &g_InVehicleContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "List Of Spawned Vehicles", &g_SpawnList));
            }
        };

        // ── Instances ─────────────────────────────────────────────────────────

        VehicleSpawnGrid g_SpawnContent{};
    }

    Vehicle::Vehicle() :
        Grid(Theme::GetContentOrigin(), 0)
    {
    }

    void Vehicle::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        constexpr int16_t kItemH = static_cast<int16_t>(Theme::kContentItemHeight);
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Spawn", &g_SpawnContent));
    }
}
