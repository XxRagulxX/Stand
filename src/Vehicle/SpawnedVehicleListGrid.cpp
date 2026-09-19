#include "Vehicle/SpawnedVehicleListGrid.hpp"

#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Script.hpp"
#include "Vehicle/SpawnedVehicleMgr.hpp"
#include "Vehicle/Vehicle.hpp"
#include "World/Self.hpp"

#include <climits>
#include <memory>

namespace Stand::Rendering
{
    namespace
    {
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
                            SpawnedVehicleMgr::Remove(handle);
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
    }

    SpawnedVehicleListGrid::SpawnedVehicleListGrid()
        : Grid(Theme::GetContentOrigin(), 0), m_LastVersion(SIZE_MAX)
    {
    }

    void SpawnedVehicleListGrid::draw()
    {
        SpawnedVehicleMgr::Prune();

        const std::size_t ver = SpawnedVehicleMgr::s_Version;
        if (ver != m_LastVersion)
        {
            m_LastVersion = ver;

            const auto& vehicles = SpawnedVehicleMgr::s_Vehicles;
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

    void SpawnedVehicleListGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        const auto& vehicles = SpawnedVehicleMgr::s_Vehicles;
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
}
