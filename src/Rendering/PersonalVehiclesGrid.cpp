#include "Rendering/PersonalVehiclesGrid.hpp"

#include "Commands/Vehicle/PersonalVehicles/CommandPersonalVehicles.hpp"
#include "Network/MPSV.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Vehicle/GarageVehicleMgr.hpp"
#include "Vehicle/PersonalVehicles.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <climits>
#include <memory>
#include <string>
#include <vector>

namespace Stand::Rendering
{
    namespace
    {
        class PVSortGrid : public Grid
        {
        public:
            PVSortGrid() : Grid(Theme::GetContentOrigin(), 0) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Name",
                    [] { Features::SetPVSort(Features::PVSort::Name); }
                ));
                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Slot",
                    [] { Features::SetPVSort(Features::PVSort::Slot); }
                ));
            }
        };
        static PVSortGrid g_SortContent{};

        class PVEntryGrid : public Grid
        {
            std::string m_RawName;
        public:
            explicit PVEntryGrid(std::string rawName)
                : Grid(Theme::GetContentOrigin(), 0), m_RawName(std::move(rawName)) {}
        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                const auto rawName = m_RawName;

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Bring",
                    [rawName] {
                        FiberPool::queueJob([rawName] {
                            auto& pvs = PersonalVehicles::GetPersonalVehicles();
                            auto it = pvs.find(rawName);
                            if (it != pvs.end())
                                it->second->Request(true);
                        });
                    }
                ));

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Save to Garage",
                    [rawName] {
                        FiberPool::queueJob([rawName] {
                            auto& pvs = PersonalVehicles::GetPersonalVehicles();
                            auto it = pvs.find(rawName);
                            if (it == pvs.end()) return;

                            auto pedPos = Self::GetPed().GetPosition();
                            const rage::fvector3 coords{ pedPos.x + 10.0f, pedPos.y, pedPos.z };
                            auto veh = it->second->Clone(coords, Self::GetPed().GetHeading());
                            if (!veh.IsValid()) return;

                            const std::string saveName = rawName.substr(0, rawName.find("##"));
                            GarageVehicleMgr::Save(saveName, veh.GetHandle());

                            int handle = veh.GetHandle();
                            ENTITY::DELETE_ENTITY(&handle);
                        });
                    }
                ));
            }
        };
    }

    PersonalVehiclesGrid::PersonalVehiclesGrid()
        : Grid(Theme::GetContentOrigin(), 0),
          m_LastVersion(SIZE_MAX),
          m_LastPVCount(SIZE_MAX),
          m_LastSort(-1)
    {}

    void PersonalVehiclesGrid::draw()
    {
        auto& pvMap             = PersonalVehicles::GetPersonalVehicles();
        const std::size_t ver   = Features::g_PVVersion;
        const std::string filter = Features::GetPVFilter();
        const int sort          = static_cast<int>(Features::GetPVSort());
        const std::size_t pvCnt = pvMap.size();

        if (ver != m_LastVersion || filter != m_LastFilter || sort != m_LastSort || pvCnt != m_LastPVCount)
        {
            m_LastVersion = ver;
            m_LastFilter  = filter;
            m_LastSort    = sort;
            m_LastPVCount = pvCnt;

            std::string lower = filter;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

            m_FilteredNames.clear();
            for (auto& [rawName, pv] : pvMap)
            {
                if (!lower.empty())
                {
                    std::string display = rawName.substr(0, rawName.find("##"));
                    std::transform(display.begin(), display.end(), display.begin(), ::tolower);
                    if (display.find(lower) == std::string::npos)
                        continue;
                }
                m_FilteredNames.push_back(rawName);
            }

            if (sort == static_cast<int>(Features::PVSort::Slot))
            {
                std::vector<std::pair<int, std::string>> slotOrder;
                slotOrder.reserve(m_FilteredNames.size());
                for (const auto& n : m_FilteredNames)
                {
                    auto it = pvMap.find(n);
                    int slot = (it != pvMap.end()) ? static_cast<int>(it->second->GetData()->PersonalVehicleSlot) : 0;
                    slotOrder.emplace_back(slot, n);
                }
                std::sort(slotOrder.begin(), slotOrder.end());
                m_FilteredNames.clear();
                for (auto& [slot, name] : slotOrder)
                    m_FilteredNames.push_back(std::move(name));
            }

            m_EntryGrids.clear();
            m_EntryGrids.reserve(m_FilteredNames.size());
            for (const auto& n : m_FilteredNames)
                m_EntryGrids.push_back(std::make_unique<PVEntryGrid>(n));

            invalidate();
        }

        Grid::draw();
    }

    void PersonalVehiclesGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);

        items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, h, "findpv"_J));
        items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, h, "Sort By",            &g_SortContent));
        items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, h, "saveallpvs"_J));

        for (std::size_t i = 0; i < m_FilteredNames.size() && i < m_EntryGrids.size(); ++i)
        {
            const std::string label = m_FilteredNames[i].substr(0, m_FilteredNames[i].find("##"));
            items_draft.push_back(std::make_unique<GridItemFolder>(
                Theme::kContentWidth, h,
                label,
                m_EntryGrids[i].get()
            ));
        }
    }
}
