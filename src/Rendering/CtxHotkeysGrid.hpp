#pragma once
#include "Rendering/StandPort/Grid.hpp"

namespace Stand { class CommandPhysical; }

namespace Stand::Rendering
{
    class CtxHotkeysGrid : public Grid
    {
    public:
        explicit CtxHotkeysGrid();

        void SetTarget(Stand::CommandPhysical* target);

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

    private:
        Stand::CommandPhysical* m_Target = nullptr;
    };
}
