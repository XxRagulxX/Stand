#pragma once
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandAestheticLightPlacement : public CommandListSelect
    {
    public:
        explicit CommandAestheticLightPlacement(CommandList* parent)
            : CommandListSelect(parent, LIT("Placement"), CMDNAMES_0(), NOLABEL,
                {
                    { 0, LIT("Character") },
                    { 1, LIT("Camera") },
                    { 2, LIT("Oval Of Lights") },
                },
                0)
        {
        }
    };
}
