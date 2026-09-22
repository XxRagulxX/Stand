#pragma once
#include "Rendering/Grid.hpp"

#include <memory>
#include <vector>

namespace Stand::Rendering
{
    class CurrentPVGrid : public Grid
    {
    public:
        CurrentPVGrid();

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
    };
}
