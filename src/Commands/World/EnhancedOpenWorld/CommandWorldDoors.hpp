#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandWorldDoors : public CommandToggle
    {
    public:
        inline static CommandWorldDoors* instance = nullptr;

        CommandToggle* blips = nullptr;

        explicit CommandWorldDoors(CommandList* parent);

        void onEnable(Click& click) override;
    };
}
