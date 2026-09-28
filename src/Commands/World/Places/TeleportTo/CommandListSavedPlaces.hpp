#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListSavedPlaces : public CommandList
    {
    public:
        explicit CommandListSavedPlaces(CommandList* parent)
            : CommandList(parent, LIT("Saved Places"))
        {
        }
    };
}
