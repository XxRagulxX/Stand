#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

#include <atomic>

namespace Stand
{
    class CommandListEditor : public CommandList
    {
    public:
        size_t spawned_offset = 0;
        std::atomic<int> spawnedVersion{0};

        explicit CommandListEditor(CommandList* parent);
    };
}
