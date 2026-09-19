#include "Rendering/VehicleSpawnGrid.hpp"

#include "Commands/Vehicle/CommandTabVehicle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/VehiclePreviewItem.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Joaat.hpp"
#include "Vehicle/SpawnedVehicleMgr.hpp"
#include "Vehicle/Vehicle.hpp"
#include "Vehicle/VehicleData.hpp"
#include "Vehicle/VehicleSpawnBehaviour.hpp"
#include "World/Self.hpp"

#include <array>
#include <cctype>
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
                    items_draft.push_back(std::make_unique<VehiclePreviewItem>(
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

        class ClassVehicleGrid : public Grid
        {
            int m_ClassIdx;
        public:
            explicit ClassVehicleGrid(int classIdx)
                : Grid(Theme::GetContentOrigin(), 0), m_ClassIdx(classIdx)
            {
            }
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                for (auto hash : g_ClassBuckets[m_ClassIdx])
                    items_draft.push_back(std::make_unique<VehiclePreviewItem>(Theme::kContentWidth, h, hash));
            }
        };

        std::vector<std::unique_ptr<ClassVehicleGrid>> g_ClassGrids;

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
                        g_ClassGrids.push_back(std::make_unique<ClassVehicleGrid>(i));
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

        // ── Spawned Vehicle List Grid ─────────────────────────────────────────

        class SpawnedVehicleEntryGrid : public Grid
        {
            int m_Handle;
        public:
            explicit SpawnedVehicleEntryGrid(int handle)
                : Grid(Theme::GetContentOrigin(), 0), m_Handle(handle)
            {
            }
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                const int handle = m_Handle;

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Drive",
                    [handle] {
                        FiberPool::queueJob([handle] {
                            Vehicle veh(handle);
                            if (!veh.IsValid()) return;
                            Script::current()->yield(200);
                            Self::GetPed().SetInVehicle(veh);
                        });
                    }
                ));

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Delete",
                    [handle] {
                        FiberPool::queueJob([handle] {
                            Vehicle veh(handle);
                            if (!veh.IsValid()) return;
                            const auto model = veh.GetModel();
                            veh.Delete();
                            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
                            Stand::SpawnedVehicleMgr::Remove(handle);
                        });
                    }
                ));

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Call To Me",
                    [handle] {
                        FiberPool::queueJob([handle] {
                            Vehicle veh(handle);
                            if (!veh.IsValid()) return;
                            veh.SetPosition(Vehicle::GetSpawnLocRelToPed(
                                Self::GetPed().GetHandle(), veh.GetModel()));
                        });
                    }
                ));
            }
        };

        class VehicleSpawnListGrid : public Grid
        {
            std::size_t m_LastVersion = SIZE_MAX;
            std::vector<std::unique_ptr<Grid>> m_EntryGrids;
        public:
            VehicleSpawnListGrid() : Grid(Theme::GetContentOrigin(), 0) {}

            void draw() override
            {
                Stand::SpawnedVehicleMgr::Prune();

                const std::size_t ver = Stand::SpawnedVehicleMgr::s_Version;
                if (ver != m_LastVersion)
                {
                    m_LastVersion = ver;

                    const auto& vehicles = Stand::SpawnedVehicleMgr::s_Vehicles;
                    m_EntryGrids.resize(vehicles.size());
                    for (std::size_t i = 0; i < vehicles.size(); ++i)
                    {
                        if (!m_EntryGrids[i])
                            m_EntryGrids[i] = std::make_unique<SpawnedVehicleEntryGrid>(vehicles[i].handle);
                    }

                    invalidate();
                }

                Grid::draw();
            }
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                const auto& vehicles = Stand::SpawnedVehicleMgr::s_Vehicles;
                for (std::size_t i = 0; i < vehicles.size() && i < m_EntryGrids.size(); ++i)
                {
                    items_draft.push_back(std::make_unique<GridItemFolder>(
                        Theme::kContentWidth,
                        static_cast<int16_t>(Theme::kContentItemHeight),
                        vehicles[i].name,
                        m_EntryGrids[i].get()
                    ));
                }
            }
        };

        // ── Search Result Grid ────────────────────────────────────────────────

        class VehicleSearchResultGrid : public Grid
        {
            std::vector<joaat_t> m_Hashes;
        public:
            VehicleSearchResultGrid() : Grid(Theme::GetContentOrigin(), 0) {}

            void SetResults(std::vector<joaat_t> hashes)
            {
                m_Hashes = std::move(hashes);
                invalidate();
            }
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                for (auto hash : m_Hashes)
                    items_draft.push_back(std::make_unique<VehiclePreviewItem>(Theme::kContentWidth, h, hash));
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
                auto& tab = Features::GetCommandTabVehicle();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.footDrive));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.footAir));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.footFront));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.footLikePV));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.footDelete));
            }
        };

        // ── In Vehicle Behaviour Grid ─────────────────────────────────────────

        class VehicleSpawnInVehicleGrid : public Grid
        {
        public:
            VehicleSpawnInVehicleGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabVehicle();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.vehDrive));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.vehAir));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.vehFront));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.vehLikePV));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.vehDelete));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.vehVelocity));
            }
        };

        // ── Instances ─────────────────────────────────────────────────────────

        VehicleSpawnDlcGrid       g_DlcContent{};
        VehicleSpawnClassGrid     g_ClassContent{};
        VehicleSpawnListGrid      g_SpawnList{};
        VehicleSearchResultGrid   g_SearchResults{};
        VehicleSpawnOnFootGrid    g_OnFootContent{};
        VehicleSpawnInVehicleGrid g_InVehicleContent{};

        // ── Search helpers ────────────────────────────────────────────────────

        std::string VehicleSuggestions(const std::string& term)
        {
            if (term.size() < 2)
                return "";

            std::string lower = term;
            for (auto& c : lower)
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

            std::string out;
            int count = 0;
            for (size_t i = 0; i < g_VehicleCount && count < 5; ++i)
            {
                std::string name(g_VehicleData[i].hash_name);
                std::string nameLower = name;
                for (auto& c : nameLower)
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                if (nameLower.find(lower) != std::string::npos)
                {
                    if (!out.empty())
                        out += "  |  ";
                    out += name;
                    ++count;
                }
            }
            return out;
        }

        void OpenInputModelName()
        {
            MenuCommandBox::Open(
                "spawn", "Input Model Name", "", "",
                [](const std::string& term) -> bool {
                    if (term.empty())
                        return false;
                    const joaat_t hash = Stand::Joaat(term.c_str());
                    if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash))
                        return false;
                    FiberPool::queueJob([hash, term] {
                        Stand::SpawnVehicle(hash, term);
                    });
                    return true;
                },
                VehicleSuggestions
            );
        }

        void OpenVehicleSearch()
        {
            MenuCommandBox::Open(
                "findvehicle", "Search for a vehicle to spawn", "", "",
                [](const std::string& term) -> bool {
                    if (term.empty())
                        return false;

                    std::string lower = term;
                    for (auto& c : lower)
                        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

                    std::vector<joaat_t> results;
                    for (size_t i = 0; i < g_VehicleCount && results.size() < 30; ++i)
                    {
                        std::string name(g_VehicleData[i].hash_name);
                        for (auto& c : name)
                            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                        if (name.find(lower) != std::string::npos)
                            results.push_back(Stand::Joaat(g_VehicleData[i].hash_name));
                    }

                    if (results.empty())
                        return false;

                    FiberPool::queueJob([results = std::move(results), term]() mutable {
                        g_SearchResults.SetResults(std::move(results));
                        MenuNavigation::Push("Search: " + term, &g_SearchResults);
                    });
                    return true;
                },
                VehicleSuggestions
            );
        }
    }

    VehicleSpawnGrid::VehicleSpawnGrid()
        : Grid(Theme::GetContentOrigin(), 0)
    {
    }

    void VehicleSpawnGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        constexpr float kItemH    = Theme::kContentItemHeight;
        constexpr int16_t kItemH16 = static_cast<int16_t>(kItemH);

        auto& settings = Features::GetCommandTabVehicle();
        items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH16, settings.manufacturerNames));
        items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH16, settings.spawngod));
        items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Search", OpenVehicleSearch));
        items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Input Model Name", OpenInputModelName));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Classes",                &g_ClassContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "DLCs",                   &g_DlcContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "On Foot Behaviour",      &g_OnFootContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "In Vehicle Behaviour",   &g_InVehicleContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "List Of Spawned Vehicles", &g_SpawnList));
    }
}
