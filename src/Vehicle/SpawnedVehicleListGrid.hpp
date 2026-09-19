#pragma once
#include "Rendering/Grid.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace Stand::Rendering
{
    class SpawnedVehicleListGrid : public Grid
    {
    public:
        SpawnedVehicleListGrid();
        void draw() override;
    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
    private:
        std::size_t m_LastVersion;
        std::vector<std::unique_ptr<Grid>> m_EntryGrids;
    };
}
