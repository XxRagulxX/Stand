#pragma once

#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/StandPort/CommandColour.hpp"
#include "Game/AbstractPlayer.hpp"

#include <SimpleMath.h>

namespace Stand
{
    class CommandEspTags : public CommandList
    {
    private:
        CommandColour* const default_colour = nullptr;

    public:
        explicit CommandEspTags(CommandList* parent, CommandColour* default_colour = nullptr)
            : CommandList(parent, LIT("Tag Colours")), default_colour(default_colour)
        {
        }

        [[nodiscard]] DirectX::SimpleMath::Color getColour(AbstractPlayer&) const
        {
            if (default_colour)
                return default_colour->getRGBA();
            return {};
        }
    };
}
