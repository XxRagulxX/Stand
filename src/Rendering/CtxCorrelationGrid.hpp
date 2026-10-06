#pragma once
#include "Rendering/StandPort/Grid.hpp"

namespace Stand { class CommandToggle; }

namespace Stand::Rendering
{
    class CtxCorrelationGrid : public Grid
    {
    public:
        explicit CtxCorrelationGrid();

        void SetTarget(Stand::CommandToggle* target);

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

    private:
        Stand::CommandToggle* m_Target = nullptr;
    };
}
