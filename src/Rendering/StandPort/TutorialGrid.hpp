#pragma once

#include "Rendering/StandPort/Grid.hpp"

namespace Stand
{
    class TutorialGrid : public Rendering::Grid
    {
    public:
        TutorialGrid();

        void populate(std::vector<std::unique_ptr<Rendering::GridItem>>& items_draft) final;
        void updateNow() {}
    };
    inline TutorialGrid g_tutorial_grid{};
}
