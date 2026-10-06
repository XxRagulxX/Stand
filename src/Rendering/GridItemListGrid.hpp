#pragma once
#include "Rendering/StandPort/Grid.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Game/typedecl.hpp"

#include <unordered_map>

namespace Stand { class GridItemList; }

namespace Stand::Rendering
{

    class GridItemListGrid : public Grid
    {
    public:
        explicit GridItemListGrid(Stand::CommandList* list);

        [[nodiscard]] Stand::CommandList* getCommandList() const noexcept { return m_List; }

        static GridItemListGrid& GetOrCreate(Stand::CommandList* list);

        void handleKey(unsigned int vkCode);

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

    private:
        Stand::CommandList* m_List;
        GridItemList* m_Item = nullptr;

        void moveCursor(int delta);
        void activateFocused();
    };
}
