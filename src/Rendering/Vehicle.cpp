#include "Rendering/Vehicle.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"
#include "Rendering/GarageGrid.hpp"
#include "Rendering/PersonalVehiclesGrid.hpp"
#include "Rendering/CurrentPVGrid.hpp"
#include "Rendering/LastVehicleGrid.hpp"
#include "Commands/Vehicle/CommandTabVehicle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Vehicle/VehicleEntityPreview.hpp"
#include "Vehicle/SpawnedVehicleListGrid.hpp"
#include "Vehicle/VehicleSpawnBehaviour.hpp"
#include "Commands/Extra/CommandFindVehicle.hpp"
#include "Menu/GUI.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Rendering/MenuCommandConsole.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Util/Joaat.hpp"
#include "Vehicle/VehicleData.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnPlate.hpp"

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

        // ── Colour Grid ───────────────────────────────────────────────────────

        class VehicleSpawnColourGrid : public Grid
        {
        public:
            VehicleSpawnColourGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabSpawnSettings();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnColour->enabled));
                AddColorCommandRows(items_draft, Theme::kContentWidth, "spawnprimarycolour"_J);
                AddColorCommandRows(items_draft, Theme::kContentWidth, "spawnsecondarycolour"_J);
            }
        };

        // ── Blips Grid ────────────────────────────────────────────────────────

        class VehicleSpawnBlipsGrid : public Grid
        {
        public:
            VehicleSpawnBlipsGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabSpawnSettings();
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnBlips->enabled));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnBlips->colour));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnBlips->scale));
                items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tab.spawnBlips->latestOnly));
                items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, h, "clearspawnedblips"_J));
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

        // ── U-key console "findvehicle" command ──────────────────────────────
        // Registers "findvehicle" in the legacy Commands registry so the
        // U-key console can find it by name (prefix-match → Tab → args-mode).
        // getArgsActivator() returns the same partial-name search callback
        // used by the "Search" menu button so both paths behave identically.

        class CmdFindVehicle : public Stand::CommandFindVehicle
        {
        public:
            CmdFindVehicle()
                : Stand::CommandFindVehicle(
                      Stand::COMMAND_ACTION, nullptr,
                      LIT("Find Vehicle"),
                      CMDNAMES("findvehicle"),
                      LIT("Search and spawn a vehicle by name."))
            {}
            std::function<void(const std::string&)> getArgsActivator() const override;
        };

        // Defined below, after VehicleSearchResultGrid so g_SearchResults is in scope.
        CmdFindVehicle g_CmdFindVehicle{};

        // ── Search Results Grid ───────────────────────────────────────────────
        // Populated by the "Search" console callback; reused across searches.

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
                    items_draft.push_back(std::make_unique<VehicleEntityPreview>(Theme::kContentWidth, h, hash));
            }
        };

        VehicleSearchResultGrid g_SearchResults{};

        // getArgsActivator() defined here — after g_SearchResults — so the
        // returned lambda can capture it by reference (it's a static).
        std::function<void(const std::string&)> CmdFindVehicle::getArgsActivator() const
        {
            return [](const std::string& term) {
                if (term.empty())
                    return;

                if (GUI::IsOpen())
                {
                    // Menu is open: show results as a preview grid the player
                    // can browse and select from (same as the "Search" button).
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
                        return;
                    g_SearchResults.SetResults(std::move(results));
                    MenuNavigation::Push("Search: " + term, &g_SearchResults);
                }
                else
                {
                    // Menu is closed (U-key only): treat the term as an exact
                    // model name, spawn immediately, and always warp the player
                    // into the driver seat.
                    const joaat_t hash = Stand::Joaat(term.c_str());
                    if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash))
                        return;
                    FiberPool::queueJob([hash, term] {
                        Stand::SpawnVehicleAndDrive(hash, term);
                    });
                }
            };
        }

        // ── Spawn Grid ────────────────────────────────────────────────────────

        VehicleSpawnDlcGrid       g_DlcContent{};
        VehicleSpawnClassGrid     g_ClassContent{};
        SpawnedVehicleListGrid    g_SpawnList{};
        VehicleSpawnColourGrid    g_ColourContent{};
        VehicleSpawnBlipsGrid     g_BlipsContent{};
        VehicleSpawnOnFootGrid    g_OnFootContent{};
        VehicleSpawnInVehicle     g_InVehicleContent{};

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
                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, kItemH,
                    "Spawned Vehicles License Plate",
                    Stand::Features::OpenSpawnPlate,
                    [] { return Stand::Features::GetSpawnPlateText(); },
                    IconSlot::Edit));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Colour Spawned Vehicles",         &g_ColourContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Blips On Spawned Vehicles",        &g_BlipsContent));
                items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Search",
                    [] {
                        MenuCommandConsole::Open(
                            "findvehicle ",
                            [](const std::string& term) {
                                if (term.empty())
                                    return;
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
                                    return;
                                g_SearchResults.SetResults(std::move(results));
                                MenuNavigation::Push("Search: " + term, &g_SearchResults);
                            },
                            "Search for a vehicle to spawn"
                        );
                    }, nullptr, IconSlot::Search));
                items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Input Model Name",
                    [] {
                        MenuCommandConsole::Open(
                            "spawn ",
                            [](const std::string& term) {
                                if (term.empty())
                                    return;
                                const joaat_t hash = Stand::Joaat(term.c_str());
                                if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash))
                                    return;
                                FiberPool::queueJob([hash, term] { Stand::SpawnVehicle(hash, term); });
                            },
                            "Input Model Name"
                        );
                    }));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Classes",                  &g_ClassContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "DLCs",                     &g_DlcContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "On Foot Behaviour",        &g_OnFootContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "In Vehicle Behaviour",     &g_InVehicleContent));
                items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "List Of Spawned Vehicles", &g_SpawnList));
            }
        };

        class LscGrid : public Grid
        {
        public:
            LscGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabVehicle();
                for (auto& child : tab.lsc->children)
                    items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, child.get()));
            }
        };

        VehicleSpawnGrid       g_SpawnContent{};
        GarageGrid             g_GarageContent{};
        PersonalVehiclesGrid   g_PersonalVehiclesContent{};
        CurrentPVGrid          g_CurrentPVContent{};
        LastVehicleGrid        g_LastVehicleContent{};
        LscGrid                g_LscContent{};

        class MovementGrid : public Grid
        {
        public:
            MovementGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabVehicle();
                for (auto& child : tab.movement->children)
                    items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, child.get()));
            }
        };

        class RocketBoostGrid : public Grid
        {
        public:
            RocketBoostGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                auto& tab = Features::GetCommandTabVehicle();
                for (auto& child : tab.rocketBoost->children)
                    items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, child.get()));
            }
        };

        MovementGrid           g_MovementContent{};
        RocketBoostGrid        g_RocketBoostContent{};
    }

    Vehicle::Vehicle() :
        Grid(Theme::GetContentOrigin(), 0)
    {
    }

    void Vehicle::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        constexpr int16_t kItemH = static_cast<int16_t>(Theme::kContentItemHeight);
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Spawn",             &g_SpawnContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Garage",            &g_GarageContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Personal Vehicles",        &g_PersonalVehiclesContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Current Personal Vehicle", &g_CurrentPVContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Last Vehicle",             &g_LastVehicleContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Los Santos Customs",       &g_LscContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Movement",                 &g_MovementContent));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Rocket Boost",              &g_RocketBoostContent));
    }
}
