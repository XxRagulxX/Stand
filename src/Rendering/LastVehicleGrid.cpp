#include "Rendering/LastVehicleGrid.hpp"

#include "Commands/Vehicle/CommandTabVehicle.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

#include <memory>
#include <vector>

namespace Stand::Rendering
{
    LastVehicleGrid::LastVehicleGrid()
        : Grid(Theme::GetContentOrigin(), 0)
    {
    }

    void LastVehicleGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
        auto& tab = Features::GetCommandTabVehicle();
        for (auto& child : tab.lastVehicle->children)
            items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, child.get()));
    }
}
