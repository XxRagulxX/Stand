#pragma once
#include "Rendering/Grid.hpp"

#include <string>
#include <unordered_map>

namespace Stand { class Command; }

namespace Stand::Rendering
{
    class CommandContextGrid : public Grid
    {
        Stand::Command* m_Command;
    public:
        explicit CommandContextGrid(Stand::Command* cmd);

        static Grid& GetOrCreate(Stand::Command* cmd);

    protected:
        void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

    private:
        static std::unordered_map<Stand::Command*, std::string>& SavedStates()
        {
            static std::unordered_map<Stand::Command*, std::string> s;
            return s;
        }

        static std::string BuildPath(Stand::Command* cmd);
    };
}
