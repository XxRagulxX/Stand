#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/World/EnhancedOpenWorld/CommandWorldDoors.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListEnhancedOpenWorld : public CommandList
    {
    private:
        class CommandDoorBlips : public CommandToggle
        {
        public:
            explicit CommandDoorBlips(CommandList* parent)
                : CommandToggle(parent, LIT("Blips"), CMDNAMES("doorblips"), NOLABEL, true)
            {
            }
        };

    public:
        explicit CommandListEnhancedOpenWorld(CommandList* parent)
            : CommandList(parent, LIT("Enhanced Open World"), CMDNAMES("enhancedopenworld"), NOLABEL)
        {
            auto* doors = createChild<CommandWorldDoors>();
            doors->blips = createChild<CommandDoorBlips>();
        }
    };
}
