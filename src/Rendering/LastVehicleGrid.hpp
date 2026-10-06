#pragma once
#include "Rendering/StandPort/Grid.hpp"

#include <memory>
#include <vector>

namespace Stand::Rendering
{
    class LastVehicleGrid : public Grid
    {
    public:
        LastVehicleGrid();

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
    };
}
