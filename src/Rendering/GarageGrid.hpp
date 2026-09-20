#pragma once
#include "Rendering/Grid.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Stand::Rendering
{
    class GarageGrid : public Grid
    {
    public:
        GarageGrid();
        void draw() override;

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

    private:
        std::size_t m_LastVersion;
        std::string m_LastFilter;
        std::vector<std::unique_ptr<Grid>> m_EntryGrids;
    };
}
