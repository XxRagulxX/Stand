#include "Rendering/GarageGrid.hpp"

#include "Commands/Vehicle/Garage/CommandGarage.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/MenuPopup.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Vehicle/GarageVehicleMgr.hpp"

#include <climits>
#include <format>
#include <memory>

namespace Stand::Rendering
{
    namespace
    {
        class GarageEntryGrid : public Grid
        {
            std::string m_Name;
        public:
            explicit GarageEntryGrid(std::string name)
                : Grid(Theme::GetContentOrigin(), 0), m_Name(std::move(name)) {}

        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
                const auto name = m_Name;

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Spawn",
                    [name] {
                        FiberPool::queueJob([name] {
                            GarageVehicleMgr::Spawn(name);
                        });
                    }
                ));

                items_draft.push_back(std::make_unique<GridItemButton>(
                    Theme::kContentWidth, h, "Delete",
                    [name] {
                        MenuPopup::Confirm(
                            std::format("Delete \"{}\" from garage?", name),
                            [name] { GarageVehicleMgr::Delete(name); }
                        );
                    }
                ));
            }
        };
    }

    GarageGrid::GarageGrid()
        : Grid(Theme::GetContentOrigin(), 0), m_LastVersion(SIZE_MAX)
    {
    }

    void GarageGrid::draw()
    {
        if (!GarageVehicleMgr::s_Loaded)
            GarageVehicleMgr::LoadList();

        const std::size_t ver    = GarageVehicleMgr::s_Version;
        const std::string filter = Features::GetGarageFilter();

        if (ver != m_LastVersion || filter != m_LastFilter)
        {
            m_LastVersion = ver;
            m_LastFilter  = filter;

            const auto filtered = GarageVehicleMgr::Filter(filter);
            m_EntryGrids.clear();
            m_EntryGrids.reserve(filtered.size());
            for (const auto& n : filtered)
                m_EntryGrids.push_back(std::make_unique<GarageEntryGrid>(n));

            invalidate();
        }

        Grid::draw();
    }

    void GarageGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);

        items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, h, "findsaved"_J));
        items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, h, "savevehicle"_J));
        items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, h, "opengaragefolder"_J));

        const auto filtered = GarageVehicleMgr::Filter(m_LastFilter);
        for (std::size_t i = 0; i < filtered.size() && i < m_EntryGrids.size(); ++i)
        {
            items_draft.push_back(std::make_unique<GridItemFolder>(
                Theme::kContentWidth, h,
                filtered[i],
                m_EntryGrids[i].get()
            ));
        }
    }
}
