#pragma once
#include "Rendering/Grid.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Stand::Rendering
{
    class PersonalVehiclesGrid : public Grid
    {
    public:
        PersonalVehiclesGrid();
        void draw() override;

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

    private:
        std::size_t              m_LastVersion;
        std::size_t              m_LastPVCount;
        std::string              m_LastFilter;
        int                      m_LastSort;
        std::vector<std::string> m_FilteredNames;
        std::vector<std::unique_ptr<Grid>> m_EntryGrids;
    };
}
